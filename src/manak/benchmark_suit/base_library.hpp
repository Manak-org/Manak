/**
 * @file manak.hpp
 * @author Sumedh Ghaisas
 *
 * Standalone header for end user.
 */
#ifndef MANAK_BASE_LIBRARY_HPP_INCLUDED
#define MANAK_BASE_LIBRARY_HPP_INCLUDED

#include <string>
#include <set>

namespace manak
{
namespace benchmark_suit
{

/**
 * This class handles all the informations about the libraries.
 */
class BaseLibrary
{
 public:
  //! Get th singleton BaseLibrary object which registers all the libraries
  //!
  //! \return BaseLibrary&
  //!
  //!
  static BaseLibrary& GetLibraryCollection()
  {
    static BaseLibrary singleton;
    return singleton;
  }

  //! Add library to the collection
  //!
  //! \param str Name of the library
  //! \return void
  //!
  //!
  void AddLibrary(const std::string& str)
  {
    lib.insert(str);
  }

 private:
  //! Set of all library name
  std::set<std::string> lib;
}; // class BaseLibrary

}; // namespace benchmark_suit
}; // namespace manak

#endif // MANAK_BASE_LIBRARY_HPP_INCLUDED
