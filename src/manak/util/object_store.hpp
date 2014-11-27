#ifndef _MANAK_UTIL_OBJECT_STORE_HPP_INCLUDED
#define _MANAK_UTIL_OBJECT_STORE_HPP_INCLUDED

#include <map>
#include <string>

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

  void Erase(const std::string& name)
  {
    auto it = this->find(name);
    if(it != this->end())
      this->erase(it);
  }
};

}
}


#endif // _MANAK_UTIL_OBJECT_STORE_HPP_INCLUDED
