#ifndef MANAK_OUTPUT_HANDLER_HPP_INCLUDED
#define MANAK_OUTPUT_HANDLER_HPP_INCLUDED

#include <iostream>
#include <map>
#include <string>

namespace manak
{

class OutputHandler
{
 public:
  OutputHandler(const std::string& filename)
    : stream(filename)
  {}

  virtual ~OutputHandler() {}

  virtual void Initialize(bool compare,
                          const std::string& c_time) = 0;

  virtual void OpenSuite(const std::string& name) = 0;

  virtual void CloseSuite() = 0;

  virtual void AddCase(const std::string& uname,
                       const std::string& name,
                       const std::map<std::string, std::list<utils::ObjectStore>>& results) = 0;

  virtual void Finalize() = 0;

 protected:
  std::ofstream stream;
};

}

#endif // MANAK_OUTPUT_HANDLER_HPP_INCLUDED
