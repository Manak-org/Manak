#ifndef MANAK_HTML_OUPUT_HANDLER_HPP_INCLUDED
#define MANAK_HTML_OUPUT_HANDLER_HPP_INCLUDED

#include <map>
#include <string>
#include <list>
#include <sstream>

#include "output_handler.hpp"
#include <manak/test_suite/test_monitor.hpp>
#include <manak/util/version.hpp>
#include <manak/util/object_store.hpp>
#include <manak/util/pmeasure.hpp>
#include <manak/util/macro_utils.hpp>
#include <manak/util/timer.hpp>

#include <manak/util/manak_env.hpp>

namespace manak
{

class HTMLOutputHandler : public OutputHandler
{
 public:
  HTMLOutputHandler(const std::string& name)
    : OutputHandler(name), total_nodes(0)
  {}

  ~HTMLOutputHandler() {}

  MANAK_INLINE void Initialize(bool compare,
                               const std::string& c_time);

  MANAK_INLINE void AddCase(const std::string& uname,
                            const std::string& name,
                            const std::map<std::string, std::list<utils::ObjectStore>>& results);

  void OpenSuite(const std::string& name)
  {
    (void)name;
  }

  void CloseSuite() {}

  MANAK_INLINE void Finalize();

 private:
  MANAK_INLINE std::string GetPMRep(const utils::ObjectStore& entry);

  std::stringstream stream1;
  std::stringstream stream2;

  size_t total_nodes;
  bool isComp;
  std::string compare_time;
};

}

#ifndef MANAK_USE_DYN_LINK
#include "html_output_handler.cpp"
#endif // MANAK_USE_DYN_LINK

#endif // MANAK_HTML_OUPUT_HANDLER_HPP_INCLUDED
