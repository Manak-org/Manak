#ifndef _MANAK_UTIL_OBJECT_STORE_HPP_INCLUDED
#define _MANAK_UTIL_OBJECT_STORE_HPP_INCLUDED

#include <map>
#include <string>
#include <list>
#include <iostream>

namespace manak
{
namespace utils
{

class ObjectStore : public std::map<std::string, void*>
{
 public:
  static ObjectStore& GetGlobalObjectStore()
  {
    static ObjectStore singleton;
    return singleton;
  }

  void* Get(const std::string& name) const
  {
    auto it = this->find(name);
    if(it != this->end())
      return it->second;

    return NULL;
  }

  void* RGet(const std::string& name)
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

  bool Erase(const std::string& name)
  {
    auto it = this->find(name);
    if(it != this->end())
    {
      this->erase(it);
      return true;
    }

    return false;
  }

  void Insert(const std::string& name, void* obj, const std::string& group_name = "")
  {
    (*this)[name] = obj;
    if(group_name != "")
      m_groups[group_name].push_back(name);
  }

  bool EraseGroup(const std::string& group_name)
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

 private:
  std::map<std::string, std::list<std::string>> m_groups;
};

std::ostream& operator<<(std::ostream& s, const ObjectStore& os)
{
  for(auto it : os)
  {
    s << it.first << " -> " << it.second << std::endl;
  }
  return s;
}

}
}


#endif // _MANAK_UTIL_OBJECT_STORE_HPP_INCLUDED
