#ifndef MANAK_MANAK_SUITE_HPP_INCLUDED
#define MANAK_MANAK_SUITE_HPP_INCLUDED

#include <string>
#include <list>
#include <map>
#include <regex>
#include <vector>

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

  ~ManakSuite();

  bool Run(const std::string& uname = "",
           const std::string& pattern = "",
           const bool compare = false);

  bool Find(const std::string& name, std::list<ManakCase*>& lbc);

  bool LoadData(const std::string& name);

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

  ManakSuite* SetCurrentSuite(const std::string& name)
  {
    auto suite = current_manak_suite->child_suits.find(name);
    if(suite != current_manak_suite->child_suits.end())
    {
      current_manak_suite = suite->second;
    }
    else
    {
      ManakSuite* n_s = new ManakSuite(name);
      current_manak_suite->AddSuite(n_s);
      current_manak_suite = n_s;
    }
    return current_manak_suite;
  }

  ManakSuite* SetCurrentSuiteToParent()
  {
    return (current_manak_suite = current_manak_suite->parent);
  }

  ManakCase* AddCase(ManakCase* obj)
  {
    auto it = children.find(obj->Name());
    if(it != children.end())
    {
      for(auto bc : it->second)
      {
        if(bc->LibraryName() == obj->LibraryName())
        {
          std::cerr << "Contains multiple entries for library "
                    << obj->LibraryName() << " with case "
                    << obj->Name() << std::endl;
          exit(1);
        }
      }
      it->second.push_back(obj);
    }
    else
    {
      children[obj->Name()].push_back(obj);
    }
    return obj;
  }

  template<typename T>
  bool AddGroup(T& gr)
  {
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

#include "manak_suite_impl.hpp"

namespace manak
{

class RegisterManakSuite
{
 public:
  RegisterManakSuite(const std::string& name)
  {
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

#define MANAK_BENCHMARK_SUITE(X)  \
( new manak::ManakSuite(#X) )

#define MANAK_AUTO_BENCHMARK_SUITE(X)  \
static manak::RegisterManakSuite STRING_JOIN(X, STRING_JOIN(invoker, __LINE__))(#X);\

#define MANAK_AUTO_BENCHMARK_SUITE_END()  \
static manak::DeRegisterManakSuite STRING_JOIN(destroy, __LINE__); \


#endif // MANAK_MANAK_SUITE_HPP_INCLUDED
