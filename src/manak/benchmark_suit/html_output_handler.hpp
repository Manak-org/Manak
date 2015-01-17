#ifndef MANAK_HTML_OUPUT_HANDLER_HPP_INCLUDED
#define MANAK_HTML_OUPUT_HANDLER_HPP_INCLUDED

#include <map>
#include <string>
#include <list>
#include <sstream>

#include "output_handler.hpp"

#include <manak/util/version.hpp>

namespace manak
{

class HTMLOutputHandler : public OutputHandler
{
 public:
  HTMLOutputHandler(const std::string& name)
    : OutputHandler(name), total_nodes(0)
  {}

  ~HTMLOutputHandler() {}

  void Initialize(const std::map<std::string, size_t>& l_map,
                  bool compare,
                  const std::string& c_time);

  void AddCase(const std::map<size_t, ManakCase*>& children,
               const std::map<size_t, std::list<utils::ObjectStore>>& results);

  void OpenSuite(const std::string& name) {}

  void CloseSuite() {}

  void Finalize();

 private:
  std::map<std::string, size_t> l_map;
  std::stringstream stream1;
  std::stringstream stream2;

  size_t total_nodes;
  bool isComp;
  std::string compare_time;
};

}

#include "html_output_handler_impl.hpp"

#endif // MANAK_HTML_OUPUT_HANDLER_HPP_INCLUDED
