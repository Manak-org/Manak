/**
 * @file simple_module_benchmark_case.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of macros used in MANAK_SIMPLE_BENCHMARK_MODULE.
 */

#ifndef MANAK_BASE_LIBRARY_NAME
#define MANAK_BASE_LIBRARY_NAME base_library
#endif // MANAK_BASE_LIBRARY_NAME

#define MANAK_BENCHMARK_CASE_ITD(Name, Function, Iter, Tolerance, Desc)       \
( new manak::BenchmarkCase(#Name,                                             \
                           MANAK_STRINGIZE(MANAK_BASE_LIBRARY_NAME),          \
                           Function,                                          \
                           Iter,                                              \
                           Tolerance,                                         \
                           Desc) )

#define MANAK_BENCHMARK_CASE(Name, Function)                                  \
MANAK_BENCHMARK_CASE_ITD(Name, Function, MANAK_DEFAULT_ITERATIONS, MANAK_DEFAULT_TOLERANCE, "")

#define MANAK_BENCHMARK_CASE_T(Name, Function, Tolerance)                     \
MANAK_BENCHMARK_CASE_ITD(NAme, Function, MANAK_DEFAULT_ITERATIONS, Tolerance, "")

#define MANAK_BENCHMARK_CASE_I(Name, Function, Iter)                          \
MANAK_BENCHMARK_CASE_ITD(Name, Function, Iter, MANAK_DEFAULT_TOLERANCE, "")


#define MANAK_AUTO_BENCHMARK_CASE_ITD(Name, Iter, Tolerance, Desc)            \
struct Name ## _ ## Benchmark                                                 \
{                                                                             \
  static void Run();                                               \
  static manak::BenchmarkCase* static_temp;                                   \
                                                                              \
};                                                                            \
manak::BenchmarkCase* Name ## _ ## Benchmark::static_temp =                   \
manak::BenchmarkSuite::GetMasterSuite()->GetCurrentSuite()->                  \
AddCase(MANAK_BENCHMARK_CASE_ITD(Name,                                        \
                                 Name ## _ ## Benchmark::Run,                 \
                                 Iter,                                        \
                                 Tolerance,                                   \
                                 Desc));                                      \
void Name ## _ ## Benchmark::Run()

#define MANAK_AUTO_BENCHMARK_CASE(Name)                                       \
MANAK_AUTO_BENCHMARK_CASE_ITD(Name, MANAK_DEFAULT_ITERATIONS, MANAK_DEFAULT_TOLERANCE, "")


#define MANAK_AUTO_BENCHMARK_CASE_T(Name, Tolerance)                          \
MANAK_AUTO_BENCHMARK_CASE_ITD(Name, MANAK_DEFAULT_ITERATIONS, Tolerance, "")

#define MANAK_AUTO_BENCHMARK_CASE_I(Name, Iter)                               \
MANAK_AUTO_BENCHMARK_CASE_TD(Name, Iter, MANAK_DEFAULT_TOLERANCE, "");

#define MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_ITD(Name, f_name, Iter, tol, Desc)      \
( manak::CTBenchmarkObject(#Name, "BaseLibrary", Iter, tol, Desc, f_name) )

#define MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_T(Name, f_name, tol)             \
( manak::CTBenchmarkObject(#Name, "BaseLibrary", MANAK_DEFAULT_ITERATIONS, tol, "", f_name) )

#define MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_I(Name, f_name, Iter)             \
( manak::CTBenchmarkObject(#Name, "BaseLibrary", Iter, MANAK_DEFAULT_TOLERANCE, "", f_name) )

#define MANAK_CREATE_BENCHMARK_WITH_TEMPLATE(Name, f_name)             \
( manak::CTBenchmarkObject(#Name, "BaseLibrary", MANAK_DEFAULT_ITERATIONS, MANAK_DEFAULT_TOLERANCE, "", f_name) )
