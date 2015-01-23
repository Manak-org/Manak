#ifndef MANAK_GROUP_HPP_INCLUDED
#define MANAK_GROUP_HPP_INCLUDED

#include <list>

#include "manak_case.hpp"
#include "t_manak_case.hpp"

namespace manak
{

template<class Derived>
class ManakGroup
{
 public:
  ManakGroup(const std::string& iden)
    : iden(iden)
  {
    InitCaller<Derived>(0);
    TDCaller<Derived>(0);
    //static_cast<Derived*>(this)->ACCaller();
  }

  template<typename abcde>
  void InitCaller(manak::utils::manak_group_test<abcde, &abcde::Manak_Group_Initialize>*)
  {
    static_cast<abcde*>(this)->Manak_Group_Initialize();
  }
  template<typename>
  void InitCaller(...)
  {}

  template<typename abcde>
  void TDCaller(manak::utils::manak_group_test<abcde, &abcde::Manak_Group_TearDown>*)
  {
    static_cast<abcde*>(this)->Manak_Group_TearDown();
  }
  template<typename>
  void TDCaller(...)
  {}

  bool AddManakCase(ManakCase* mc)
  {
    cases.push_back(mc);
    return true;
  }

  std::list<manak::ManakCase*> cases;
  std::string iden;
};

}


#endif // MANAK_GROUP_HPP_INCLUDED
