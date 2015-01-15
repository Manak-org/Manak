#ifndef MANAK_MACROS_HPP_INCLUDED
#define MANAK_MACROS_HPP_INCLUDED

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


#endif // MANAK_MACROS_HPP_INCLUDED
