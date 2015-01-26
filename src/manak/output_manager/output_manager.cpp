#ifdef MANAK_USE_DYN_LINK
#include <manak/util/macro_utils.hpp>
#include <manak/util/object_store.hpp>
#include "output_manager.hpp"
#endif // MANAK_USE_DYM_LINK

namespace manak
{

MANAK_INLINE void OutputManager::AddHandler(OutputHandler* handler)
{
  handlers.push_back(handler);
}

MANAK_INLINE void OutputManager::Initialize(bool compare,
                                            const std::string& c_time)
{
  for(auto oh : handlers)
  {
    oh->Initialize(compare, c_time);
  }
}

MANAK_INLINE void OutputManager::AddCase(const std::string& uname,
                                         const std::string& name,
                                         const std::map<std::string, std::list<utils::ObjectStore>>& results)
{
  for(auto oh : handlers)
  {
    oh->AddCase(uname, name, results);
  }
}

MANAK_INLINE void OutputManager::Finalize()
{
  for(auto oh : handlers)
  {
    oh->Finalize();
  }
}

}
