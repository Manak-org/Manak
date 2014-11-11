#ifndef MANAK_BENCHMARK_SUITE_HPP_INCLUDED
#define MANAK_BENCHMARK_SUITE_HPP_INCLUDED

#include <string>
#include <list>
#include <map>
#include <regex>
#include <vector>

#include <manak/util/macro_utils.hpp>
#include <manak/util/log.hpp>

#include "base_library.hpp"
#include "node.hpp"
#include "pmeasure.hpp"
#include "benchmark_case.hpp"

namespace manak
{

class BenchmarkSuite
{
 public:
  BenchmarkSuite()
    : current_benchmark_suite(this),
    parent(NULL){}

  BenchmarkSuite(const std::string& name)
    : name(name),
    current_benchmark_suite(this),
    parent(NULL) {}

  static BenchmarkSuite* GetMasterSuite()
  {
    static BenchmarkSuite singleton;
    return &singleton;
  }

  BenchmarkSuite* GetCurrentSuite() const
  {
    return current_benchmark_suite;
  }

  BenchmarkSuite* SetCurrentSuite(BenchmarkSuite* suite)
  {
    return (current_benchmark_suite = suite);
  }

  BenchmarkSuite* SetCurrentSuite(const std::string& name)
  {
    auto suite = current_benchmark_suite->child_suits.find(name);
    if(suite != current_benchmark_suite->child_suits.end())
    {
      current_benchmark_suite = suite->second;
      std::cout << "Benchmark set to " << current_benchmark_suite << std::endl;
    }
    else
    {
      BenchmarkSuite* n_s = new BenchmarkSuite(name);
      current_benchmark_suite->AddSuite(n_s);
      current_benchmark_suite = n_s;
      std::cout << "Benchmark set to " << current_benchmark_suite << std::endl;
    }
    return current_benchmark_suite;
  }

  BenchmarkSuite* SetCurrentSuiteToParent()
  {
    return (current_benchmark_suite = current_benchmark_suite->parent);
  }

  const std::string& Name() const
  {
    return name;
  }
  std::string& Name()
  {
    return name;
  }

  BenchmarkCase* AddCase(BenchmarkCase* obj)
  {
    auto it = children.find(obj->Name());
    if(it != children.end())
    {
      for(auto bc : it->second)
      {
        if(bc->LibraryName() == obj->LibraryName())
        {
          std::cerr << "Contains multiple entries for library "
                    << obj->LibraryName() << " with benchmark case "
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

  BenchmarkSuite* AddSuite(BenchmarkSuite* suite)
  {
    child_suits[suite->Name()] = suite;
    suite->parent = this;
    return suite;
  }

  bool Run(const std::string& uname = "",
           const std::string& pattern = "",
           const bool compare = false)
  {
    std::string n_pattern = "";
    std::string c_pattern = "";
    if(pattern != "")
    {
      size_t index = pattern.find("/");
      if(index == std::string::npos)
      {
        c_pattern = pattern;
      }
      else
      {
        c_pattern = pattern.substr(0, index);
        n_pattern = pattern.substr(index + 1);
      }
    }

    std::regex r1(c_pattern);

    std::cout << c_pattern << std::endl;

    for(auto cases : children)
    {
      if((n_pattern == "" && c_pattern == "") || (n_pattern == "" && std::regex_match(cases.first, r1)))
      {
        utils::CaseLogEntry& cle = utils::Log::GetLog().Add(cases.first, uname + "/" + cases.first);

        for(auto c : cases.second)
        {
          c->Run(cle, compare);
        }
      }
    }
    for(auto it : child_suits)
    {
      if(c_pattern == "" || std::regex_match(it.first, r1))
        it.second->Run(uname + "/" + name, n_pattern, compare);
    }
    return true;
  }

  bool Find(const std::string& name, std::list<BenchmarkCase*>& lbc)
  {
    size_t t = name.find("/", 1);
    if(t == std::string::npos)
    {
      auto it = children.find(name.substr(1));
      if(it != children.end())
      {
        lbc = it->second;
        return true;
      }
      else return false;
    }
    else
    {
      auto it = child_suits.find(name.substr(1, t - 1));
      if(it != child_suits.end())
      {
        return it->second->Find(name.substr(t), lbc);
      }
      else return false;
    }
  }

  bool LoadData(const std::string& name)
  {
    std::ifstream stream(name);

    if(!stream.is_open())
    {
      std::cerr << "Unable to open file " << name << std::endl;
    }

    std::vector<std::string> libs;

    size_t s_libs;

    stream >> s_libs;
    for(size_t i = 0;i < s_libs;i++)
    {
      std::string temp;
      stream >> temp;
      libs.push_back(temp);
    }

    size_t s_cases;
    stream >> s_cases;
    for(size_t i = 0;i < s_cases;i++)
    {
      std::string name;
      stream >> name;
      size_t s_entries;
      stream >> s_entries;

      std::list<BenchmarkCase*> lbc;
      if(Find(name, lbc))
      {
        for(size_t i = 0;i < s_entries;i++)
        {
          for(auto l_name : libs)
          {
            double c_value;
            stream >> c_value;

            for(auto bc : lbc)
            {
              if(bc->LibraryName() == l_name)
              {
                bc->AddComparisonEntry(c_value);
                break;
              }
            }
          }
        }
      }
    }
    return true;
  }

  ~BenchmarkSuite()
  {
    for(auto lbc : children)
    {
      for(auto bc : lbc.second)
      {
        delete bc;
      }
    }
    for(auto bs : child_suits)
    {
      delete bs.second;
    }
  }

 private:
  std::string name;

  std::map<std::string, std::list<BenchmarkCase*>> children;
  std::map<std::string, BenchmarkSuite*> child_suits;

  BenchmarkSuite* current_benchmark_suite;
  BenchmarkSuite* parent;
};

class RegisterBenchmarkSuite
{
 public:
  RegisterBenchmarkSuite(const std::string& name)
  {
    BenchmarkSuite::GetMasterSuite()->SetCurrentSuite(name);
  }
};

class DeRegisterBenchmarkSuite
{
 public:
  DeRegisterBenchmarkSuite()
  {
    BenchmarkSuite::GetMasterSuite()->SetCurrentSuiteToParent();
  }
};

}

#define MANAK_BENCHMARK_SUITE(X)  \
( new BenchmarkSuite(#X) )

#define MANAK_AUTO_BENCHMARK_SUITE(X)  \
namespace X \
{ \
static manak::RegisterBenchmarkSuite STRING_JOIN(X, STRING_JOIN(invoker, __LINE__))(#X);\

#define MANAK_AUTO_BENCHMARK_SUITE_END()  \
static manak::DeRegisterBenchmarkSuite STRING_JOIN(destroy, __LINE__); \
};

#ifndef MANAK_ALTERNATE_INIT_FUNCTION
namespace manak
{

bool init_benchmarking_module()
{
  #ifdef MANAK_SIMPLE_BENCHMARK_MODULE
  manak::BenchmarkSuite::GetMasterSuite()->Name() = MANAK_STRINGIZE(MANAK_SIMPLE_BENCHMARK_MODULE);
  manak::benchmark_suit::BaseLibrary::GetLibraryCollection().AddLibrary(MANAK_STRINGIZE(Base_Library));

  #else // MANAK_SIMPLE_BENCHMARK_MODULE
  manak::BenchmarkSuite::GetMasterSuite()->Name() = MANAK_STRINGIZE(MANAK_SIMPLE_BENCHMARK_MODULE);

  #endif // MANAK_SIMPLE_BENCHMARK_MODULE

  #ifndef MANAK_SIMPLE_BENCHMARK_MODULE
  #ifndef MANAK_BENCHMARK_MODULE
  static_assert(false, "Manak benchmarking module not defined. Use either MANAK_BENCHMARK_MODULE or MANAK_SIMPLE_BENCHMARK_MODULE");
  #endif // MANAK_BENCHMARK_MODULE
  #endif // MANAK_SIMPLE_BENCHMARK_MODULE

  return true;
}
}
#endif // MANAK_AUTO_BENCHMARK_MAIN

#endif // MANAK_BENCHMARK_SUITE_HPP_INCLUDED
