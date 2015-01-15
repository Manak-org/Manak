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

  virtual void Initialize(const std::map<std::string, size_t>& l_map,
                          bool compare,
                          const std::string& c_time) = 0;

  virtual void OpenSuite(const std::string& name) = 0;

  virtual void CloseSuite() = 0;

  virtual void AddCase(const std::map<size_t, BenchmarkCase*>& children,
                       const std::map<size_t, std::list<utils::ObjectStore>>& results) = 0;

  virtual void Finalize() = 0;

  std::string GetPMRep(const utils::ObjectStore& entry);

 protected:
  std::ofstream stream;
};

}

#include "output_handler_impl.hpp"

#endif // MANAK_OUTPUT_HANDLER_HPP_INCLUDED
