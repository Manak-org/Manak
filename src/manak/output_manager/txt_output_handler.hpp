#ifndef MANAK_TXT_OUPUT_HANDLER_HPP_INCLUDED
#define MANAK_TXT_OUPUT_HANDLER_HPP_INCLUDED

#include <map>
#include <string>
#include <list>

#include "output_handler.hpp"

#include <manak/test_suite/test_monitor.hpp>
#include <manak/util/version.hpp>
#include <manak/util/object_store.hpp>
#include <manak/util/pmeasure.hpp>
#include <manak/util/macro_utils.hpp>

namespace manak
{

class TXTOutputHandler : public OutputHandler
{
 public:
  TXTOutputHandler(const std::string& name)
    : OutputHandler(name)
  {}

  ~TXTOutputHandler() {}

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

  void Finalize() {}

 private:
  std::string GetPMRep(const utils::ObjectStore& entry);
};

}

#ifndef MANAK_USE_DYN_LINK
#include "txt_output_handler.cpp"
#endif // MANAK_USE_DYN_LINK

#endif // MANAK_TXT_OUPUT_HANDLER_HPP_INCLUDED
