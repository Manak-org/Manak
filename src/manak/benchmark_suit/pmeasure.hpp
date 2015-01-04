/**
 * @file pmeasure.hpp
 * @author Sumedh Ghaisas
 *
 * Definition of PMeasure.
 */
#ifndef MANAK_PMEASURE_HPP_INCLUDED
#define MANAK_PMEASURE_HPP_INCLUDED

#include <iostream>
#include <sstream>

namespace manak
{

/**
 * Stores the timing information of any run.
 */
struct PMeasure
{
  //! Create empty object
  //! For STL library use
  //!
  //!
  PMeasure()
    : min(0), max(0), avg(0) {}

  //! Generate PMeasure object with given time information
  //!
  //! \param min Minimum time observed
  //! \param max Maximum time observed
  //! \param avg Average Time
  //!
  //!
  PMeasure(double min, double max, double avg)
    : min(min), max(max), avg(avg) {}

  //! Compare the average with given value taking into consideration the tolerance
  int Compare(const double value, const double tolerance)
  {
    if(avg - value > tolerance)
      return 1;
    if(value - avg > tolerance)
      return -1;
    return 0;
  }

  //! Minimum time
  double min;
  //! Maximum time
  double max;
  //! Average time
  double avg;
};

inline std::ostream& operator<<(std::ostream& stream, const PMeasure& pm)
{
  std::stringstream s;
  s << pm.avg;
  stream << s.str();

  return stream;
}; // struct PMeasure

}; // namespace manak


#endif // MANAK_PMEASURE_HPP_INCLUDED
