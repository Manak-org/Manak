#ifdef MANAK_USE_DYN_LINK
#include "timer.hpp"
#endif // MANAK_USE_DYN_LINK

namespace manak
{

//! Initialize a new Timer instance
MANAK_INLINE void Timer::Initialize()
{
  TotalTime() = 0;
  Min() = std::numeric_limits<uint64_t>::max();
  Max() = std::numeric_limits<uint64_t>::min();
  StateInit() = true;
}

//! Deinitialize and register
MANAK_INLINE PMeasure Timer::Deinitialize(size_t iterations)
{
  if(StateInit())
  {
    StateInit() = false;

    PMeasure pm = GetStats(iterations);

    return pm;
  }
  return PMeasure();
}

  //! Mark start of the iteration
MANAK_INLINE void Timer::StartIter()
{
  if(StateInit())
  {
    CTime() = 0;
    StateIter() = true;
  }
}

//! Start timer
MANAK_INLINE void Timer::StartTimer()
{
  if(StateIter())
  {
    Start() = microtimer();
    StateTimer() = true;
  }
}

//! Stop timer
MANAK_INLINE void Timer::StopTimer()
{
  if(StateTimer())
  {
    CTime() += microtimer() - Start();
    StateTimer() = false;
  }
}

//! Mark end of the iteration
MANAK_INLINE bool Timer::EndIter(size_t& iterations)
{
  if(StateIter() == false)
    return false;
  StateIter() = false;

  TotalTime() += CTime();

  if(Min() > CTime())
    Min() = CTime();
  if(Max() < CTime())
    Max() = CTime();

  iterations--;
  if(iterations <= 0) return false;
  return true;
}

//! Get statistics of current timer instance
MANAK_INLINE PMeasure Timer::GetStats(size_t iterations)
{
  return PMeasure(Min(), Max(), (double)TotalTime()/ iterations);
}

//! Get-set total time
MANAK_INLINE uint64_t& Timer::TotalTime()
{
  static uint64_t singleton;
  return singleton;
}

//! Get-set current timing
MANAK_INLINE uint64_t& Timer::CTime()
{
  static uint64_t singleton;
  return singleton;
}

//! Get-set start time
MANAK_INLINE uint64_t& Timer::Start()
{
  static uint64_t singleton;
  return singleton;
}

//! Get-set min time
MANAK_INLINE uint64_t& Timer::Min()
{
  static uint64_t singleton;
  return singleton;
}

//! Get-set max time
MANAK_INLINE uint64_t& Timer::Max()
{
  static uint64_t singleton;
  return singleton;
}

//! Get-set iteration state
MANAK_INLINE bool& Timer::StateIter()
{
  static bool singleton;
  return singleton;
}

//! Get-set timer state
MANAK_INLINE bool& Timer::StateTimer()
{
  static bool singleton;
  return singleton;
}

//! Get-set initialization state
MANAK_INLINE bool& Timer::StateInit()
{
  static bool singleton;
  return singleton;
}

//! Replace all occurrences
MANAK_INLINE void Timer::replaceAll(std::string& str,
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
MANAK_INLINE std::string Timer::getTimeStamp(bool removeSpaces)
{
  time_t tm;
  time(&tm);
  std::string timestamp = ctime(&tm);
  timestamp.pop_back();
  timestamp = timestamp.substr(4);
  if(removeSpaces) replaceAll(timestamp, " ", "_");
  return timestamp;
}

}
