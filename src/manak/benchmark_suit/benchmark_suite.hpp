#ifndef MANAK_BENCHMARK_SUITE_HPP_INCLUDED
#define MANAK_BENCHMARK_SUITE_HPP_INCLUDED

#include <string>
#include <list>
#include <map>

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

  bool Run(const std::string& pattern = "")
  {
    for(auto cases : children)
    {
      utils::CaseLogEntry& cle = utils::Log::GetLog().Add(cases.first);

      for(auto c : cases.second)
      {
        c->Run(cle);
      }
    }
    for(std::map<std::string, BenchmarkSuite*>::iterator it = child_suits.begin();it != child_suits.end();it++)
    {
      std::cout << it->second << " " << it->second->Name() << std::endl;
      it->second->Run();
    }
    return true;
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
