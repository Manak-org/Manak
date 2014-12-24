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

#include "benchmark_case.hpp"

#include <manak/util/macro_utils.hpp>

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

  bool Run(const std::string& uname = "",
           const std::string& pattern = "",
           const bool compare = false);

  bool Find(const std::string& name, std::list<BenchmarkCase*>& lbc);

  bool LoadData(const std::string& name);

  ~BenchmarkSuite();

  static BenchmarkSuite* GetMasterSuite()
  {
    static BenchmarkSuite singleton;
    return &singleton;
  }

  BenchmarkSuite* AddSuite(BenchmarkSuite* suite)
  {
    child_suits[suite->Name()] = suite;
    suite->parent = this;
    return suite;
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

  const std::string& Name() const
  {
    return name;
  }
  std::string& Name()
  {
    return name;
  }

 private:
  std::string name;

  std::map<std::string, std::list<BenchmarkCase*>> children;
  std::map<std::string, BenchmarkSuite*> child_suits;

  BenchmarkSuite* current_benchmark_suite;
  BenchmarkSuite* parent;
};

bool init_benchmarking_module()
{
  #ifndef MANAK_SIMPLE_BENCHMARK_MODULE
  #ifndef MANAK_BENCHMARK_MODULE
  static_assert(false, "Manak benchmarking module not defined. Use either MANAK_BENCHMARK_MODULE or MANAK_SIMPLE_BENCHMARK_MODULE");
  #endif // MANAK_BENCHMARK_MODULE
  #endif // MANAK_SIMPLE_BENCHMARK_MODULE

  manak::BenchmarkSuite::GetMasterSuite()->Name() = MANAK_MODULE_NAME;

  return true;
}

}

#include "pmeasure.hpp"
#include "run_tree.hpp"

#include "benchmark_suite_impl.hpp"

namespace manak
{

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

#endif // MANAK_BENCHMARK_SUITE_HPP_INCLUDED
