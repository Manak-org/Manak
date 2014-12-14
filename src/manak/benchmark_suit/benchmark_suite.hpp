/**
 * @file benchmark_suite.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration BenchmarkSuite.
 */
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
#include "pmeasure.hpp"
#include "benchmark_case.hpp"
#include "run_tree.hpp"

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
    }
    else
    {
      BenchmarkSuite* n_s = new BenchmarkSuite(name);
      current_benchmark_suite->AddSuite(n_s);
      current_benchmark_suite = n_s;
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
    RunTree::GlobalRunTree().AddSuite(name);

    std::regex r1(pattern);

    for(auto cases : children)
    {
      std::string match_string = "";
      if(uname == "")
        match_string += cases.first;
      else match_string = uname + "/" + cases.first;

      if((pattern == "") || std::regex_match(match_string, r1))
      {
        utils::CaseLogEntry& cle = utils::Log::GetLog().Add(cases.first, uname + "/" + cases.first);

        for(auto c : cases.second)
        {
          RunTree::GlobalRunTree().AddCase(c);
          //c->Run(cle, compare);
        }
      }
    }
    for(auto it : child_suits)
    {
      it.second->Run(uname + "/" + name, pattern, compare);
    }

    RunTree::GlobalRunTree().CloseSuite();

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
      std::cerr << "Unable to open file " << name << " for comparison" << std::endl;
      return false;
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
          size_t s_measures;
          stream >> s_measures;

          for(size_t i = 0;i < s_measures;i++)
          {
            double c_value;
            stream >> c_value;

            for(auto bc : lbc)
            {
              if(bc->LibraryName() == libs[i])
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
( new manak::BenchmarkSuite(#X) )

#define MANAK_AUTO_BENCHMARK_SUITE(X)  \
static manak::RegisterBenchmarkSuite STRING_JOIN(X, STRING_JOIN(invoker, __LINE__))(#X);\

#define MANAK_AUTO_BENCHMARK_SUITE_END()  \
static manak::DeRegisterBenchmarkSuite STRING_JOIN(destroy, __LINE__); \

namespace manak
{

bool init_benchmarking_module()
{
  #ifdef MANAK_SIMPLE_BENCHMARK_MODULE
  manak::BenchmarkSuite::GetMasterSuite()->Name() = MANAK_STRINGIZE(MANAK_SIMPLE_BENCHMARK_MODULE);

  #else // MANAK_SIMPLE_BENCHMARK_MODULE
  manak::BenchmarkSuite::GetMasterSuite()->Name() = MANAK_STRINGIZE(MANAK_BENCHMARK_MODULE);

  #endif // MANAK_SIMPLE_BENCHMARK_MODULE

  #ifndef MANAK_SIMPLE_BENCHMARK_MODULE
  #ifndef MANAK_BENCHMARK_MODULE
  static_assert(false, "Manak benchmarking module not defined. Use either MANAK_BENCHMARK_MODULE or MANAK_SIMPLE_BENCHMARK_MODULE");
  #endif // MANAK_BENCHMARK_MODULE
  #endif // MANAK_SIMPLE_BENCHMARK_MODULE

  return true;
}
}

#endif // MANAK_BENCHMARK_SUITE_HPP_INCLUDED
