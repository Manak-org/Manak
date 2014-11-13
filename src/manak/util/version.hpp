/**
 * @file version.hpp
 * @author Sumedh Ghaisas
 *
 * Version Information.
 */
#ifndef MANAK_UTIL_VERSION_HPP_INCLUDED
#define MANAK_UTIL_VERSION_HPP_INCLUDED

#include <iostream>
#include <string>
#include <sstream>

//! Declare version
#define __MANAK_VERSION_MAJOR "master"
#define __MANAK_VERSION_MINOR 0
#define __MANAK_VERSION_PATCH 0

namespace manak
{

//! Returns the version information in string
std::string GetVersionInfo()
{
  std::stringstream stream;
  stream << "Manak C++ Benchmarking Library" << std::endl;
  stream << "version " << __MANAK_VERSION_MAJOR << "." << __MANAK_VERSION_MINOR;
  stream << "." << __MANAK_VERSION_PATCH;
  return stream.str();
}

}; // namespace manak


#endif // MANAK_UTIL_VERSION_HPP_INCLUDED
