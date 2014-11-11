#ifndef MANAK_UTIL_TIMER_HPP_INCLUDED
#define MANAK_UTIL_TIMER_HPP_INCLUDED

#include <limits>
#include <iostream>

#include <manak/benchmark_suit/pmeasure.hpp>

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

  static void replaceAll(std::string& str,
                         const std::string& from,
                         const std::string& to)
  {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
      while((start_pos = str.find(from, start_pos)) != std::string::npos)
      {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
      }
      start_pos += to.length();
    }
  }

  //! Returns current time-stamp as string
  //!
  //! \return std::string
  //!
  //!
  static std::string getTimeStamp(bool removeSpaces = false)
  {
    time_t tm;
    time(&tm);
    std::string timestamp = ctime(&tm);
    timestamp.pop_back();
    timestamp = timestamp.substr(4);
    if(removeSpaces) replaceAll(timestamp, " ", "_");
    return timestamp;
  }

  static void Initialize(size_t iter)
  {
    TotalTime() = 0;
    Min() = std::numeric_limits<uint64_t>::max();
    Max() = std::numeric_limits<uint64_t>::min();
    Iterations() = iter;
    CIter() = Iterations();
  }

  static void Reinitialize()
  {
    TotalTime() = 0;
    Min() = std::numeric_limits<uint64_t>::max();
    Max() = std::numeric_limits<uint64_t>::min();
    CIter() = Iterations();
  }

  static void StartIter()
  {
    CTime() = 0;
    StateIter() = true;
  }

  static void StartTimer()
  {
    Start() = microtimer();
    StateTimer() = true;
  }

  static void StopTimer()
  {
    if(StateTimer())
    {
      CTime() += microtimer() - Start();
      StateTimer() = false;
    }
  }

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

  static PMeasure GetStats()
  {
    return PMeasure(Min(), Max(), (double)TotalTime()/ Iterations());
  }

  static uint64_t& TotalTime()
  {
    static uint64_t singleton;
    return singleton;
  }

  static uint64_t& CTime()
  {
    static uint64_t singleton;
    return singleton;
  }

  static uint64_t& Start()
  {
    static uint64_t singleton;
    return singleton;
  }

  static uint64_t& Min()
  {
    static uint64_t singleton;
    return singleton;
  }
  static uint64_t& Max()
  {
    static uint64_t singleton;
    return singleton;
  }

  static size_t& Iterations()
  {
    static size_t singleton;
    return singleton;
  }

  static int& CIter()
  {
    static int singleton;
    return singleton;
  }

  static bool& StateIter()
  {
    static bool singleton;
    return singleton;
  }

  static bool& StateTimer()
  {
    static bool singleton;
    return singleton;
  }
}; // class Timer

}; // namespace manak

#endif // MANAK_UTIL_TIMER_HPP_INCLUDED
