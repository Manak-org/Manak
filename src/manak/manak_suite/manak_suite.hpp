#ifndef MANAK_MANAK_SUITE_HPP_INCLUDED
#define MANAK_MANAK_SUITE_HPP_INCLUDED

#include <string>
#include <list>
#include <map>
#include <regex>
#include <vector>
#include <fstream>

#include "manak_case.hpp"
#include "t_manak_case.hpp"

#include <manak/util/macro_utils.hpp>

namespace manak
{

class ManakSuite
{
 public:
  ManakSuite()
    : current_manak_suite(this),
    parent(NULL){}

  ManakSuite(const std::string& name)
    : name(name),
    current_manak_suite(this),
    parent(NULL) {}

  MANAK_INLINE ~ManakSuite();

  MANAK_INLINE bool Run(const std::string& uname = "",
                        const std::string& pattern = "",
                        const bool compare = false);

  MANAK_INLINE bool Find(const std::string& name, std::list<ManakCase*>& lbc);

  MANAK_INLINE bool LoadData(const std::string& name);

  static ManakSuite& GetMasterSuite()
  {
    static ManakSuite singleton;
    return singleton;
  }

  ManakSuite* AddSuite(ManakSuite* suite)
  {
    child_suits[suite->Name()] = suite;
    suite->parent = this;
    return suite;
  }

  ManakSuite* GetCurrentSuite() const
  {
    return current_manak_suite;
  }

  ManakSuite* SetCurrentSuite(ManakSuite* suite)
  {
    return (current_manak_suite = suite);
  }

  MANAK_INLINE ManakSuite* SetCurrentSuite(const std::string& name);

  ManakSuite* SetCurrentSuiteToParent()
  {
    return (current_manak_suite = current_manak_suite->parent);
  }

  MANAK_INLINE ManakCase* AddCase(ManakCase* obj);

  template<typename T, typename... Args>
  bool AddGroup(Args... args)
  {
    T& gr = T::Global();
    gr.InitCaller(args...);
    gr.ACCaller();
    for(auto it : gr.cases)
    {
      AddCase(it);
    }
    gr.cases.clear();
    return true;
  }

  const std::string& Name() const
  {
    return name;
  }
  std::string& Name()
  {
    return name;
  }
  const std::string& UName() const
  {
    return uname;
  }
  std::string& UName()
  {
    return uname;
  }

 private:
  std::string name;
  std::string uname;

  std::map<std::string, std::list<ManakCase*>> children;
  std::map<std::string, ManakSuite*> child_suits;

  ManakSuite* current_manak_suite;
  ManakSuite* parent;
};

}

#include "result_collector.hpp"

#ifndef MANAK_USE_DYN_LINK
#include "manak_suite.cpp"
#endif // MANAK_USE_DYN_LINK

namespace manak
{

class RegisterManakSuite
{
 public:
  RegisterManakSuite(const std::string& name)
  {
    if(name == "")
    {
      std::cerr << "Invalid suite name!!" << std::endl;
      exit(1);
    }
    ManakSuite::GetMasterSuite().SetCurrentSuite(name);
  }
};

class DeRegisterManakSuite
{
 public:
  DeRegisterManakSuite()
  {
    ManakSuite::GetMasterSuite().SetCurrentSuiteToParent();
  }
};

}

#endif // MANAK_MANAK_SUITE_HPP_INCLUDED
