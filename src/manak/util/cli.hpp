#ifndef MANAK_UTIL_CLI_HPP_INCLUDED
#define MANAK_UTIL_CLI_HPP_INCLUDED

#include <string>
#include <algorithm>

namespace manak
{
namespace utils
{
namespace cli
{

class CLI
{
 public:
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

  static bool cmdOptionExists(char** begin, char** end, const std::string& option)
  {
    return std::find(begin, end, option) != end;
  }
};

}
}
}


#endif // MANAK_UTIL_CLI_HPP_INCLUDED
