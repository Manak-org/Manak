#ifndef MANAK_ENV_HPP_INCLUDED
#define MANAK_ENV_HPP_INCLUDED

#include <string>

namespace manak
{

class ManakEnv
{
 public:
  static ManakEnv& GlobalEnv()
  {
    static ManakEnv singleton;
    return singleton;
  }

  const std::string& GetModuleName() const
  {
    return module_name;
  }
  std::string& GetModuleName()
  {
    return module_name;
  }

 private:
  std::string module_name;
};

}


#endif // MANAK_ENV_HPP_INCLUDED
