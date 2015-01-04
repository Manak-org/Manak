/**
 * @file benchmark_case.hpp
 * @author Sumedh Ghaisas
 * @author Harman Singh
 *
 * Declaration of BenchmarkCase.
 */
#ifndef MANAK_BENCHMARK_CASE_HPP_INCLUDED
#define MANAK_BENCHMARK_CASE_HPP_INCLUDED

#include <string>
#include <functional>
#include <stdint.h>
#include <limits>
#include <fstream>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <list>

#include "pmeasure.hpp"

#include <manak/util/timer.hpp>
#include <manak/util/macro_utils.hpp>
#include <manak/util/template_utils.hpp>
#include <manak/util/object_store.hpp>

namespace manak
{

class BenchmarkCase
{
 public:
  BenchmarkCase(const std::string& name,
                const std::string& library_name,
                const size_t iterations = MANAK_DEFAULT_ITERATIONS,
                double tolerance = MANAK_DEFAULT_TOLERANCE,
                const std::string& desc = "")
    : name(name),
    library_name(library_name),
    desc(desc),
    tolerance(tolerance),
    iterations(iterations)
  {}

  template<typename T>
  BenchmarkCase(const std::string& name,
                const std::string& library_name,
                std::function<T()> run_function,
                const size_t iterations =MANAK_DEFAULT_ITERATIONS,
                double tolerance = MANAK_DEFAULT_TOLERANCE,
                const std::string& desc = "")
    : name(name),
    library_name(library_name),
    desc(desc),
    tolerance(tolerance),
    iterations(iterations)
  {
    run_functions.emplace_back([=](){run_function();});
  }

  template<typename T>
  BenchmarkCase(const std::string& name,
                const std::string& library_name,
                T (run_function)(),
                const size_t iterations =MANAK_DEFAULT_ITERATIONS,
                double tolerance = MANAK_DEFAULT_TOLERANCE,
                const std::string& desc = "")
    : name(name),
    library_name(library_name),
    desc(desc),
    tolerance(tolerance),
    iterations(iterations)
  {
    std::function<T()> fun(run_function);
    run_functions.emplace_back("", [=](){fun();});
  }

  std::list<utils::ObjectStore> Run();

  void AddComparisonEntry(double d)
  {
    to_c.push_back(d);
  }

  const std::string& Name() const
  {
    return name;
  }

  const std::string& LibraryName() const
  {
    return library_name;
  }
  const std::string& UName() const
  {
    return uname;
  }
  std::string& UName()
  {
    return uname;
  }


 protected:
  std::string name;
  std::string uname;
  std::string library_name;
  std::string desc;
  std::list<std::pair<std::string, std::function<void()>>> run_functions;
  double tolerance;
  size_t iterations;

  std::vector<double> to_c;
};

template<typename RType, typename... Args>
class T_Benchmark_Case : public BenchmarkCase
{
 public:
  T_Benchmark_Case()
    : BenchmarkCase("", "", 0, 0, ""){}

  T_Benchmark_Case(const std::string& name,
                   const std::string& library_name,
                   std::function<RType(Args...)> t_function,
                   const size_t iterations = MANAK_DEFAULT_ITERATIONS,
                   double tolerance = MANAK_DEFAULT_TOLERANCE,
                   const std::string& desc = "")
    : BenchmarkCase(name, library_name, iterations, tolerance, desc),
    t_function(t_function)
  {

  }

  T_Benchmark_Case<RType, Args...>* AddArgs(Args... args)
  {
    std::tuple<Args...> temp(args...);
    run_functions.emplace_back("", [=](){utils::Caller(t_function, temp); });
    return this;
  }

  T_Benchmark_Case* AddArgs_N(const std::string& name, Args... args)
  {
    std::tuple<Args...> temp(args...);
    run_functions.emplace_back(name, [=](){utils::Caller(t_function, temp); });
    return this;
  }

  template<typename... Args2>
  T_Benchmark_Case* AddCustomArgs(std::function<std::list<std::tuple<Args...>>(Args2...)> fun, Args2... params)
  {
    std::list<std::tuple<Args...>> args = fun(params...);
    for(auto s_arg : args)
    {
      utils::Caller(utils::BindToObject(&T_Benchmark_Case<RType, Args...>::AddArgs, this), s_arg);
    }
    return this;
  }

  template<typename... Args2>
  T_Benchmark_Case* AddCustomArgs(std::list<std::tuple<Args...>> (&fun)(Args2...), Args2... params)
  {
    std::list<std::tuple<Args...>> args = fun(params...);
    for(auto s_arg : args)
    {
      utils::Caller(utils::BindToObject(&T_Benchmark_Case<RType, Args...>::AddArgs, this), s_arg);
    }
    return this;
  }

  template<typename ST, typename... Args2>
  T_Benchmark_Case* AddCustomArgs_N(std::list<std::tuple<ST, Args...>> (&fun)(Args2...), Args2... params)
  {
    std::list<std::tuple<ST, Args...>> args = fun(params...);
    for(auto s_arg : args)
    {
      utils::Caller(utils::BindToObject(&T_Benchmark_Case<RType, Args...>::AddArgs_N, this), s_arg);
    }
    return this;
  }

private:
  std::function<RType(Args...)> t_function;
  size_t iter;
};

template<typename RType, typename... Args>
T_Benchmark_Case<RType, Args...>* CTBenchmarkObject(const std::string& name,
                                                    const std::string& library_name,
                                                    const size_t iterations,
                                                    double tolerance,
                                                    const std::string& desc,
                                                    RType (*fun)(Args...))
{
  return new T_Benchmark_Case<RType, Args...>(name, library_name, std::function<RType(Args...)>(fun), iterations, tolerance, desc);
}

template<typename RType, typename... Args>
T_Benchmark_Case<RType, Args...>* CTBenchmarkObject(const std::string& name,
                                                    const std::string& library_name,
                                                    const size_t iterations,
                                                    double tolerance,
                                                    const std::string& desc,
                                                    std::function<RType(Args...)> fun)
{
  return new T_Benchmark_Case<RType, Args...>(name, library_name, fun, iterations, tolerance, desc);
}

}

//! Add implementation
#include "benchmark_case_impl.hpp"

#ifdef MANAK_SIMPLE_BENCHMARK_MODULE
#include "simple_module_benchmark_case.hpp"

#else // MANAK_SIMPLE_BENCHMARK_MODULE
#include "module_benchmark_case.hpp"

#endif // MANAK_SIMPLE_BENCHMARK_MODULE

#define MANAK_ADD_BENCHMARK(bench)                                            \
class STRING_JOIN(unamed, STRING_JOIN(_, __LINE__))                           \
{                                                                             \
  static manak::BenchmarkCase* static_temp;                                   \
};                                                                            \
manak::BenchmarkCase*                                                         \
STRING_JOIN(unamed, STRING_JOIN(_, __LINE__))::static_temp =                  \
manak::BenchmarkSuite::GetMasterSuite().GetCurrentSuite()->AddCase(bench)


#define Measure(Code)                                                         \
manak::Timer::StartTimer();                                                   \
Code                                                                          \
manak::Timer::StopTimer();

#define Repeat(Code)                                                          \
manak::Timer::Initialize();                                                   \
do                                                                            \
{                                                                             \
  manak::Timer::StartIter();                                                  \
  manak::Timer::StartTimer();                                                 \
                                                                              \
  Code                                                                        \
                                                                              \
  manak::Timer::StopTimer();                                                  \
}while(manak::Timer::EndIter());                                              \
manak::Timer::Deinitialize();

#define SetIter(X)                                                            \
if((size_t*)manak::utils::ObjectStore::GetGlobalObjectStore().Get("Timer_CurrentSubIterations") != NULL) \
  delete (size_t*)manak::utils::ObjectStore::GetGlobalObjectStore().Get("Timer_CurrentSubIterations");                                                              \
if(X != 0)                                                                    \
{                                                                             \
  manak::utils::ObjectStore::GetGlobalObjectStore().Insert("Timer_CurrentSubIterations", new size_t(X)); \
}

#define SetTol(X)                                                             \
if((size_t*)manak::utils::ObjectStore::GetGlobalObjectStore().Get("Timer_CurrentSubTolerance") != NULL) \
  delete (double*)manak::utils::ObjectStore::GetGlobalObjectStore().Get("Timer_CurrentSubTolerance");                                                              \
manak::utils::ObjectStore::GetGlobalObjectStore().Insert("Timer_CurrentSubTolerance", new double(X)); \


#endif // MANAK_BENCHMARK_CASE_HPP_INCLUDED
