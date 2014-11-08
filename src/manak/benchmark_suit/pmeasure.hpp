/**
 * @file pmeasure.hpp
 * @author Sumedh Ghaisas
 *
 * Definition of PMeasure.
 */
#ifndef MANAK_PMEASURE_HPP_INCLUDED
#define MANAK_PMEASURE_HPP_INCLUDED

#include <iostream>

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

  //! Minimum time
  double min;
  //! Maximum time
  double max;
  //! Average time
  double avg;
};

inline std::ostream& operator<<(std::ostream& s, const PMeasure& pm)
{
  s << pm.avg << " " << pm.min << " " << pm.max;
  return s;
}

}


#endif // MANAK_PMEASURE_HPP_INCLUDED
