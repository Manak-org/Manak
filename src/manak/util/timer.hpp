/**
 * @file timer.hpp
 * @author Sumedh Ghaisas
 * @author Harman Singh
 *
 * Declaration of Timer.
 */
#ifndef MANAK_UTIL_TIMER_HPP_INCLUDED
#define MANAK_UTIL_TIMER_HPP_INCLUDED

#include <limits>
#include <iostream>
#include <vector>

#include <manak/benchmark_suit/pmeasure.hpp>
#include <manak/util/log.hpp>
#include <manak/util/object_store.hpp>

#if defined(__unix__) || defined(__unix)
  #include <time.h>       // clock_gettime()
  #include <sys/time.h>   // timeval, gettimeofday()
  #include <unistd.h>     // flags like  _POSIX_VERSION

#elif defined(__MACH__) && defined(__APPLE__)
  #include <mach/mach_time.h>   // mach_timebase_info,
                                // mach_absolute_time()
  // TEMPORARY
  #include <time.h>       // clock_gettime()
  #include <sys/time.h>   // timeval, gettimeofday()
  #include <unistd.h>     // flags like  _POSIX_VERSION

#elif defined(_WIN32)
  #include <windows.h>  //GetSystemTimeAsFileTime(),
                        // QueryPerformanceFrequency(),
                        // QueryPerformanceCounter()
  #include <winsock.h>  //timeval on windows

  // uint64_t isn't defined on every windows.
  #if !defined(HAVE_UINT64_T)
    #if SIZEOF_UNSIGNED_LONG == 8
      typedef unsigned long uint64_t;
    #else
      typedef unsigned long long  uint64_t;
    #endif  // SIZEOF_UNSIGNED_LONG
  #endif  // HAVE_UINT64_T

  //gettimeofday has no equivalent will need to write extra code for that.
  #if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
    #define DELTA_EPOCH_IN_MICROSECS 11644473600000000Ui64
  #else
    #define DELTA_EPOCH_IN_MICROSECS 11644473600000000ULL
  #endif // _MSC_VER, _MSC_EXTENSIONS

  #ifdef min
    #undef min
  #endif // min
  #ifdef max
    #undef max
  #endif // max
#else
  #error "unknown OS"
#endif


// On Windows machines, we need to define timersub.
#ifdef _WIN32
inline void timersub(const timeval* tvp, const timeval* uvp, timeval* vvp)
{
  vvp->tv_sec = tvp->tv_sec - uvp->tv_sec;
  vvp->tv_usec = tvp->tv_usec - uvp->tv_usec;
  if (vvp->tv_usec < 0)
  {
     --vvp->tv_sec;
     vvp->tv_usec += 1000000;
  }
}
#endif

namespace manak
{

/**
 * This class handles all the time related queries of libGDL.
 */
class Timer
{
 public:
  //! Returns the time in microseconds
  //!
  //! \return uint64_t Time in microseconds
  //!
  //!
  static uint64_t microtimer()
  {
    timeval tv;

    #if defined(__unix__) || defined(__unix)
    gettimeofday (&tv, NULL);

    #elif defined(_WIN32)
    static double frequency = 0.0;
    static LARGE_INTEGER offset;

    // If this is the first time we've run, get the frequency.
    // We use frequency == 0.0 to indicate that
    // QueryPerformanceFrequency is uninitialised.
    if (frequency == 0.0)
    {
      LARGE_INTEGER pF;
      if (!QueryPerformanceFrequency(&pF))
      {
        // Fallback for the QueryPerformanceCounter function.
        //FileTimeToTimeVal(tv);
      }
      else
      {
        QueryPerformanceCounter(&offset);
        frequency = (double)pF.QuadPart / 1000000.0;
      }
    }

    if (frequency != 0.0)
    {
      LARGE_INTEGER pC;
      // Get the current performance-counter value.
      QueryPerformanceCounter(&pC);

      pC.QuadPart -= offset.QuadPart;
      double microseconds = (double)pC.QuadPart / frequency;
      pC.QuadPart = microseconds;
      tv.tv_sec = (long)pC.QuadPart / 1000000;
      tv.tv_usec = (long)(pC.QuadPart % 1000000);
    }

    #endif

    return tv.tv_sec * 1000000 + tv.tv_usec;
  }

  //! Initialize a new Timer instance
  static void Initialize(size_t iter)
  {
    TotalTime() = 0;
    Min() = std::numeric_limits<uint64_t>::max();
    Max() = std::numeric_limits<uint64_t>::min();
    Iterations() = iter;
    CIter() = Iterations();
    StateInit() = true;
  }

  //! Re initialize the current timer instance
  static void Reinitialize()
  {
    TotalTime() = 0;
    Min() = std::numeric_limits<uint64_t>::max();
    Max() = std::numeric_limits<uint64_t>::min();
    CIter() = Iterations();
    StateInit() = true;
  }

  //! Deinitialize and register
  static void Deinitialize()
  {
    utils::ObjectStore& os = utils::ObjectStore::GetGlobalObjectStore();

    if(StateInit())
    {
      StateInit() = false;

      std::string sub_name = "";
      std::string* temp = (std::string*)os.RGet("Timer_CurrentSubName");
      if(temp)
        sub_name = *temp;

      size_t& current_index = *(size_t*)os.Get("Timer_CurrentIndex");

      utils::CaseLogEntry& cle = *(utils::CaseLogEntry*)os.Get("Timer_CurrentCaseLogEntry");

      utils::LogEntry& le = cle.Add(current_index, sub_name);

      PMeasure pm = GetStats();

      double tolerance = *(double*)os.Get("Timer_CurrentTolerance");
      size_t l_id = *(size_t*)os.Get("Timer_CurrentLibraryID");
      std::vector<double> to_c = *(std::vector<double>*)os.Get("Timer_CurrentToCompare");

      double comp_val = -1;
      if(current_index < to_c.size())
        comp_val = to_c[current_index];

      bool current_compare_stat = *(bool*)os.Get("Timer_CurrentCompareStat");

      if(current_compare_stat)
        le.Add(pm, l_id);
      else
      {
        if(comp_val != -1)
          le.Add(pm, l_id, pm.Compare(comp_val, tolerance), comp_val);
        else
          le.Add(pm, l_id);
      }
      current_index++;
    }
  }

  //! Mark start of the iteration
  static void StartIter()
  {
    if(StateInit())
    {
      CTime() = 0;
      StateIter() = true;
    }
  }

  //! Start timer
  static void StartTimer()
  {
    if(StateIter())
    {
      Start() = microtimer();
      StateTimer() = true;
    }
  }

  //! Stop timer
  static void StopTimer()
  {
    if(StateTimer())
    {
      CTime() += microtimer() - Start();
      StateTimer() = false;
    }
  }

  //! Mark end of the iteration
  static bool EndIter()
  {
    if(StateIter() == false)
      return false;
    StateIter() = false;

    TotalTime() += CTime();

    if(Min() > CTime())
      Min() = CTime();
    if(Max() < CTime())
      Max() = CTime();

    CIter()--;
    if(CIter() <= 0) return false;
    return true;
  }

  //! Get statistics of current timer instance
  static PMeasure GetStats()
  {
    return PMeasure(Min(), Max(), (double)TotalTime()/ Iterations());
  }

  //! Get-set total time
  static uint64_t& TotalTime()
  {
    static uint64_t singleton;
    return singleton;
  }

  //! Get-set current timing
  static uint64_t& CTime()
  {
    static uint64_t singleton;
    return singleton;
  }

  //! Get-set start time
  static uint64_t& Start()
  {
    static uint64_t singleton;
    return singleton;
  }

  //! Get-set min time
  static uint64_t& Min()
  {
    static uint64_t singleton;
    return singleton;
  }
  //! Get-set max time
  static uint64_t& Max()
  {
    static uint64_t singleton;
    return singleton;
  }

  //! Get-set number
  static size_t& Iterations()
  {
    static size_t singleton;
    return singleton;
  }

  //! Get-set number of iterations
  static int& CIter()
  {
    static int singleton;
    return singleton;
  }

  //! Get-set iteration state
  static bool& StateIter()
  {
    static bool singleton;
    return singleton;
  }

  //! Get-set timer state
  static bool& StateTimer()
  {
    static bool singleton;
    return singleton;
  }

  //! Get-set initialization state
  static bool& StateInit()
  {
    static bool singleton;
    return singleton;
  }
}; // class Timer

}; // namespace manak

#endif // MANAK_UTIL_TIMER_HPP_INCLUDED
