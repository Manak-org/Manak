#ifndef MANAK_OUTPUT_MANAGER_HPP_INCLUDED
#define MANAK_OUTPUT_MANAGER_HPP_INCLUDED

#include <map>
#include <list>
#include <fstream>

#include <manak/util/object_store.hpp>
#include <manak/util/macro_utils.hpp>

#include "output_handler.hpp"
#include "txt_output_handler.hpp"
#include "html_output_handler.hpp"

namespace manak
{

class OutputManager
{
 public:
  static OutputManager& GlobalOutputManager()
  {
    static OutputManager singleton;
    return singleton;
  }

  ~OutputManager()
  {
    for(auto oh : handlers)
    {
      delete oh;
    }
  }

  MANAK_INLINE void AddHandler(OutputHandler* handler);

  MANAK_INLINE void Initialize(bool compare,
                               const std::string& c_time);

  MANAK_INLINE void OpenSuite(const std::string& name);

  MANAK_INLINE void CloseSuite();

  MANAK_INLINE void AddCase(const std::string& uname,
                            const std::string& name,
                            const std::map<std::string, std::list<utils::ObjectStore>>& results);

  MANAK_INLINE void Finalize();

 private:
  std::list<OutputHandler*> handlers;
};

}

#ifndef MANAK_USE_DYN_LINK
#include "output_manager.cpp"
#endif // MANAK_USE_DYM_LINK

#endif // MANAK_OUTPUT_MANAGER_HPP_INCLUDED
