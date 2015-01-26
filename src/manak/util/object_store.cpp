#include <iostream>
#include <string>

#ifdef MANAK_USE_DYN_LINK
#include "object_store.hpp"
#endif // MANAK_USE_DYN_LINK

namespace manak
{
namespace utils
{

MANAK_INLINE ObjectStore& ObjectStore::GetGlobalObjectStore()
{
  static ObjectStore singleton;
  return singleton;
}

MANAK_INLINE void* ObjectStore::Get(const std::string& name) const
{
  auto it = this->find(name);
  if(it != this->end())
    return it->second;

  return NULL;
}

MANAK_INLINE void* ObjectStore::RGet(const std::string& name)
{
  void* out = NULL;
  auto it = this->find(name);
  if(it != this->end())
  {
    out = it->second;
    this->erase(it);
  }
  return out;
}

MANAK_INLINE bool ObjectStore::Erase(const std::string& name)
{
  auto it = this->find(name);
  if(it != this->end())
  {
    this->erase(it);
    return true;
  }

  return false;
}

MANAK_INLINE void ObjectStore::Insert(const std::string& name, void* obj, const std::string& group_name)
{
  (*this)[name] = obj;
  if(group_name != "")
    m_groups[group_name].push_back(name);
}

MANAK_INLINE bool ObjectStore::EraseGroup(const std::string& group_name)
{
  auto g_it = m_groups.find(group_name);
  if(g_it != m_groups.end())
  {
    for(auto name : g_it->second)
    {
      auto it = this->find(name);
      if(it != this->end())
        this->erase(it);
    }
    m_groups.erase(g_it);
    return true;
  }
  return false;
}

MANAK_INLINE std::ostream& operator<<(std::ostream& s, const ObjectStore& os)
{
  for(auto it : os)
  {
    s << it.first << " -> " << it.second << std::endl;
  }
  return s;
}

}
}
