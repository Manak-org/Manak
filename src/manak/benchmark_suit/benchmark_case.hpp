#ifndef MANAK_BENCHMARK_CASE_HPP_INCLUDED
#define MANAK_BENCHMARK_CASE_HPP_INCLUDED

#include <string>
#include <functional>
#include <inttypes.h>
#include <limits>
#include <fstream>
#include <iostream>
#include <tuple>
#include <type_traits>

#include "node.hpp"
#include "pmeasure.hpp"
#include "base_library.hpp"

#include <manak/util/timer.hpp>
#include <manak/util/macro_utils.hpp>
#include <manak/util/template_utils.hpp>

#ifndef MANAK_DEFAULT_TOLERANCE
  #define MANAK_DEFAULT_TOLERANCE 10
#endif

#ifndef MANAK_DEFAULT_ITERATIONS
  #define MANAK_DEFAULT_ITERATIONS 10
#endif

//! While the benchmark is running all the output on std::cout and std::cerr
//! is redirected to another stream
//! If that stream is not defined then define it ourselves
#ifndef MANAK_BENCHMARK_REDIRECTION_STREAM

//! Open the logging file stream
#define MANAK_OPEN_LOG std::ofstream f("benchmark_log.txt", std::fstream::app);
#define MANAK_BENCHMARK_REDIRECTION_STREAM f.rdbuf()

#endif

#ifndef MANAK_OPEN_LOG
#define MANAK_OPEN_LOG
#endif


namespace manak
{

class BenchmarkCase
{
 public:
  BenchmarkCase(const std::string& name,
                const std::string& library_name,
                const size_t iterations =MANAK_DEFAULT_ITERATIONS,
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
    run_functions.emplace_back([=](){fun();});
  }

  const std::string& Name() const
  {
    return name;
  }

  const std::string& LibraryName() const
  {
    return library_name;
  }

  bool Run()
  {
    for(auto run_function : run_functions)
    {
      MANAK_OPEN_LOG;
      std::streambuf *coutbuf = std::cout.rdbuf();
      std::streambuf *cerrbuf = std::cout.rdbuf();

      std::cout.rdbuf(MANAK_BENCHMARK_REDIRECTION_STREAM);
      std::cerr.rdbuf(MANAK_BENCHMARK_REDIRECTION_STREAM);

      std::cout << "m3" <<std::endl;

      Timer::Initialize(iterations);

      do
      {
        std::cout << "m4" << std::endl;
        Timer::StartIter();
        Timer::StartTimer();

        std::cout << "m4.5" << std::endl;

        run_function();

        std::cout << "m5" << std::endl;

        Timer::StopTimer();
      }while(Timer::EndIter());

      std::cout.rdbuf(coutbuf);
      std::cerr.rdbuf(cerrbuf);

      std::cout << "m6" << std::endl;

      f.close();

      PMeasure pm = Timer::GetStats();
      std::cout << pm << std::endl;
    }
    return true;
  }

 protected:
  std::string name;
  std::string library_name;
  std::string desc;
  std::list<std::function<void()>> run_functions;
  double tolerance;
  size_t iterations;
};

template<int ...>
struct seq { };

template<int N, int ...S>
struct gens : gens<N-1, N-1, S...> { };

template<int ...S>
struct gens<0, S...> {
  typedef seq<S...> type;
};

template<typename T>
class T_Benchmark_Case : public BenchmarkCase
{
 public:
  T_Benchmark_Case()
    : BenchmarkCase("", "", 0, 0, ""){}

  T_Benchmark_Case(const std::string& name,
                   const std::string& library_name,
                   std::function<T> t_function,
                   const size_t iterations = MANAK_DEFAULT_ITERATIONS,
                   double tolerance = MANAK_DEFAULT_TOLERANCE,
                   const std::string& desc = "")
    : BenchmarkCase(name, library_name, iterations, tolerance, desc),
    t_function(t_function)
  {

  }

  template<typename... Args>
  T_Benchmark_Case* AddArgs(Args... args)
  {
    std::tuple<Args...> temp(args...);
    run_functions.emplace_back([=](){return utils::Caller(t_function, temp); });
    return this;
  }

  template<typename... Args>
  T_Benchmark_Case* AddCustomArgs(std::function<std::list<std::tuple<Args...>>()> fun)
  {
    std::list<std::tuple<Args...>> args = fun();
    for(auto s_arg : args)
    {
      utils::Caller(utils::BindToObject(&T_Benchmark_Case::AddArgs<Args...>, this), s_arg);
    }
    return this;
  }

 private:
  std::function<T> t_function;
  size_t iter;
};

template<typename RType, typename... Args>
T_Benchmark_Case<RType(Args...)>* CTBenchmarkObject(const std::string& name,
                                                    const std::string& library_name,
                                                    const size_t iterations,
                                                    double tolerance,
                                                    const std::string& desc,
                                                    RType (&fun)(Args...))
{
  return new T_Benchmark_Case<RType(Args...)>(name, library_name, std::function<RType(Args...)>(fun), iterations, tolerance, desc);
}

}

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
manak::BenchmarkSuite::GetMasterSuite()->GetCurrentSuite()->AddCase(bench)


#define Measure(Code)                                                         \
manak::Timer::StartTimer();                                                   \
Code                                                                          \
manak::Timer::StopTimer();

#define Repeat(Code)                                                          \
manak::Timer::Reinitialize();                                                 \
do                                                                            \
{                                                                             \
  manak::Timer::StartIter();                                                  \
  manak::Timer::StartTimer();                                                 \
                                                                              \
  Code                                                                        \
                                                                              \
  manak::Timer::StopTimer();                                                  \
}while(manak::Timer::EndIter());


#define MANAK_CREATE_BENCHMARK_CASE_FUNCTION(Function, ...)                   \
[=]() {                                                                       \
        Function(__VA_ARGS__);                                                \
      }


#endif // MANAK_BENCHMARK_CASE_HPP_INCLUDED
