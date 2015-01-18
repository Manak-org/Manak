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
#include <limits>
#include <list>

#include <manak/manak_suite/manak_case.hpp>
#include <manak/util/pmeasure.hpp>

#include <manak/util/timer.hpp>
#include <manak/util/macro_utils.hpp>
#include <manak/util/template_utils.hpp>
#include <manak/util/object_store.hpp>

namespace manak
{

class BenchmarkCase : public ManakCase
{
 public:
  BenchmarkCase(const std::string& name,
                const std::string& library_name,
                double tolerance = MANAK_DEFAULT_TOLERANCE,
                const size_t iterations = MANAK_DEFAULT_ITERATIONS,
                const double success_p = MANAK_DEFAULT_SP)
    : ManakCase(name, library_name, tolerance, iterations, success_p)
  {}

  template<typename T>
  BenchmarkCase(const std::string& name,
                const std::string& library_name,
                std::function<T()> run_function,
                double tolerance = MANAK_DEFAULT_TOLERANCE,
                const size_t iterations = MANAK_DEFAULT_ITERATIONS,
                const double success_p = MANAK_DEFAULT_SP)
    : ManakCase(name, library_name, run_function, tolerance, iterations, success_p)
  {}

  template<typename T>
  BenchmarkCase(const std::string& name,
                const std::string& library_name,
                T (run_function)(),
                 double tolerance = MANAK_DEFAULT_TOLERANCE,
                const size_t iterations = MANAK_DEFAULT_ITERATIONS,
                const double success_p = MANAK_DEFAULT_SP)
    : ManakCase(name, library_name, run_function, tolerance, iterations, success_p)
  {}

  std::list<utils::ObjectStore> Run();
};

}

//! Add implementation
#include "benchmark_case_impl.hpp"

#endif // MANAK_BENCHMARK_CASE_HPP_INCLUDED
