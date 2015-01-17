#ifndef MANAK_TXT_OUPUT_HANDLER_HPP_INCLUDED
#define MANAK_TXT_OUPUT_HANDLER_HPP_INCLUDED

#include <map>
#include <string>
#include <list>

#include "output_handler.hpp"

#include <manak/util/version.hpp>

namespace manak
{

class TXTOutputHandler : public OutputHandler
{
 public:
  TXTOutputHandler(const std::string& name)
    : OutputHandler(name)
  {}

  ~TXTOutputHandler() {}

  void Initialize(const std::map<std::string, size_t>& l_map,
                  bool compare,
                  const std::string& c_time);

  void AddCase(const std::map<size_t, ManakCase*>& children,
               const std::map<size_t, std::list<utils::ObjectStore>>& results);

  void OpenSuite(const std::string& name) {}

  void CloseSuite() {}

  void Finalize() {}

 private:
  std::map<std::string, size_t> l_map;
};

}

#include "txt_output_handler_impl.hpp"


#endif // MANAK_TXT_OUPUT_HANDLER_HPP_INCLUDED
