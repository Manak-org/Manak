/**
 * @file cli.hpp
 * @author Sumedh Ghaisas
 *
 * Command Line Interface.
 */
#ifndef MANAK_UTIL_CLI_HPP_INCLUDED
#define MANAK_UTIL_CLI_HPP_INCLUDED

#include <string>
#include <algorithm>

namespace manak
{
namespace utils /** Useful Utilities **/
{
namespace cli /** Command Line Interface. **/
{

/**
 * Command Line Interface
 */
class CLI
{
 public:
  //! Check if certain option is passed
  static char* getCmdOption(char ** begin,
                            char ** end,
                            const std::string & option)
  {
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
  }

  //! Get passed option
  static bool cmdOptionExists(char** begin, char** end, const std::string& option)
  {
    return std::find(begin, end, option) != end;
  }
}; // class CLI

}; // namespace cli
}; // namespace utils
}; // namespace manak


#endif // MANAK_UTIL_CLI_HPP_INCLUDED
