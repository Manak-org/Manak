#ifndef MANAK_BENCHMARK_CASE_HPP_INCLUDED
#define MANAK_BENCHMARK_CASE_HPP_INCLUDED

#include <string>
#include <functional>
#include <stdint.h>
//#include <inttypes.h>
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
#include <manak/util/log.hpp>

//! Set default tolerance if not defined
#ifndef MANAK_DEFAULT_TOLERANCE
  #define MANAK_DEFAULT_TOLERANCE 10
#endif

//! Set default iterations if not defined
#ifndef MANAK_DEFAULT_ITERATIONS
  #define MANAK_DEFAULT_ITERATIONS 10
#endif

#ifndef MANAK_REDIRECTION_FILENAME

#define MANAK_REDIRECTION_FILENAME benchmark_log.txt

#endif // MANAK_REDIRECTION_FILENAME

//! While the benchmark is running all the output on std::cout and std::cerr
//! is redirected to another stream
//! If that stream is not defined then define it ourselves
#ifndef MANAK_BENCHMARK_REDIRECTION_STREAM

//! Open the logging file stream
#define MANAK_OPEN_LOG std::ofstream f(MANAK_STRINGIZE(MANAK_REDIRECTION_FILENAME), std::fstream::app);
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
    run_functions.emplace_back("", [=](){fun();});
  }

  const std::string& Name() const
  {
    return name;
  }

  const std::string& LibraryName() const
  {
    return library_name;
  }

  bool Run(utils::CaseLogEntry& cle,
           const bool compare = false)
  {
    size_t l_id = utils::Log::GetLog().AddLibrary(library_name);

    size_t index = 0;
    for(auto run_function : run_functions)
    {
      MANAK_OPEN_LOG;
      std::streambuf *coutbuf = std::cout.rdbuf();
      std::streambuf *cerrbuf = std::cout.rdbuf();

      std::cout.rdbuf(MANAK_BENCHMARK_REDIRECTION_STREAM);
      std::cerr.rdbuf(MANAK_BENCHMARK_REDIRECTION_STREAM);

      Timer::Initialize(iterations);

      do
      {
        Timer::StartIter();
        Timer::StartTimer();

        run_function.second();

        Timer::StopTimer();
      }while(Timer::EndIter());

      std::cout.rdbuf(coutbuf);
      std::cerr.rdbuf(cerrbuf);

      f.close();

      PMeasure pm = Timer::GetStats();

      utils::LogEntry& le = cle.Add(index, run_function.first);
      if(!compare)
        le.Add(pm, l_id);
      else
      {
        if(to_c.size() > index)
          le.Add(pm, l_id, pm.Compare(to_c[index], tolerance), to_c[index]);
        else
          le.Add(pm, l_id);
      }
      index++;
    }
    return true;
  }

  void AddComparisonEntry(double d)
  {
    to_c.push_back(d);
  }

 protected:
  std::string name;
  std::string library_name;
  std::string desc;
  std::list<std::pair<std::string, std::function<void()>>> run_functions;
  double tolerance;
  size_t iterations;

  std::vector<double> to_c;
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
    run_functions.emplace_back("", [=](){utils::Caller(t_function, temp); });
    return this;
  }

  template<typename... Args>
  T_Benchmark_Case* AddArgs_N(const std::string& name, Args... args)
  {
    std::tuple<Args...> temp(args...);
    run_functions.emplace_back(name, [=](){utils::Caller(t_function, temp); });
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
