#ifndef _MANAK_UTIL_OBJECT_STORE_HPP_INCLUDED
#define _MANAK_UTIL_OBJECT_STORE_HPP_INCLUDED

#include <map>
#include <string>
#include <list>
#include <iostream>

#include "macro_utils.hpp"

namespace manak
{
namespace utils
{

class ObjectStore : public std::map<std::string, void*>
{
 public:
  MANAK_INLINE static ObjectStore& GetGlobalObjectStore();

  MANAK_INLINE void* Get(const std::string& name) const;

  MANAK_INLINE void* RGet(const std::string& name);

  MANAK_INLINE bool Erase(const std::string& name);

  MANAK_INLINE void Insert(const std::string& name, void* obj,
                          const std::string& group_name = "");

  MANAK_INLINE bool EraseGroup(const std::string& group_name);

 private:
  std::map<std::string, std::list<std::string>> m_groups;
};

MANAK_INLINE std::ostream& operator<<(std::ostream& s, const ObjectStore& os);

}
}

#ifndef MANAK_USE_DYN_LINK
#include "object_store.cpp"
#endif // MANAK_USE_DYN_LINK

#endif // _MANAK_UTIL_OBJECT_STORE_HPP_INCLUDED
