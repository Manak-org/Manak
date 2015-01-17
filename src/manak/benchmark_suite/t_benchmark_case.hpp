#ifndef MANAK_T_BENCHMARK_CASE_HPP_INCLUDED
#define MANAK_T_BENCHMARK_CASE_HPP_INCLUDED

#include "benchmark_case.hpp"

#include <manak/manak_suite/t_manak_case.hpp>

namespace manak
{

template<typename RType, typename... Args>
using T_Benchmark_Case = TManakCase<BenchmarkCase, RType, Args...>;

}

#endif // MANAK_T_BENCHMARK_CASE_HPP_INCLUDED
