/**
 * @file simple_module_benchmark_case.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of macros used in MANAK_SIMPLE_BENCHMARK_MODULE.
 */

#ifndef MANAK_BASE_LIBRARY_NAME
#define MANAK_BASE_LIBRARY_NAME MANAK_SIMPLE_MODULE
#endif // MANAK_BASE_LIBRARY_NAME

////////////////////////////////////////////////////////////////////////////////
/// MANAK_BENCHMARK_CASE macros
////////////////////////////////////////////////////////////////////////////////

#define MANAK_BENCHMARK_CASE_TIS(Name, Function, Tol, Iter, SP)               \
_MANAK_BENCHMARK_CASE_TIS(Name, MANAK_BASE_LIBRARY_NAME, Function, Tol, Iter, SP)

#define MANAK_BENCHMARK_CASE_TI(Name, Function, Tol, Iter)                    \
MANAK_BENCHMARK_CASE_TIS(Name, Function, Tol, Iter, MANAK_DEFAULT_SP)

#define MANAK_BENCHMARK_CASE_IS(Name, Function, Iter, SP)                     \
MANAK_BENCHMARK_CASE_TIS(Name, Function, MANAK_DEFAULT_TOLERANCE, Iter, SP)

#define MANAK_BENCHMARK_CASE_I(Name, Function, Iter)                          \
MANAK_BENCHMARK_CASE_TI(Name, Function, Iter, MANAK_DEFAULT_TOLERANCE)

#define MANAK_BENCHMARK_CASE_T(Name, Function, Tol)                           \
MANAK_BENCHMARK_CASE_TI(Name, Function, Tol, MANAK_DEFAULT_ITERATIONS)

#define MANAK_BENCHMARK_CASE(Name, Function)                                  \
MANAK_BENCHMARK_CASE_T(Name, Function, MANAK_DEFAULT_TOLERANCE)


////////////////////////////////////////////////////////////////////////////////
/// MANAK_AUTO_BENCHMARK_CASE macros
////////////////////////////////////////////////////////////////////////////////

#define MANAK_AUTO_BENCHMARK_CASE_TIS(Name, Tol, Iter, SP)                    \
_MANAK_AUTO_BENCHMARK_CASE_TIS(Name, MANAK_BASE_LIBRARY_NAME, Tol, Iter, SP)

#define MANAK_AUTO_BENCHMARK_CASE_TI(Name, Tol, Iter)                         \
MANAK_AUTO_BENCHMARK_CASE_TIS(Name, Tol, Iter, MANAK_DEFAULT_SP)

#define MANAK_AUTO_BENCHMARK_CASE_IS(Name, Iter, SP)                          \
MANAK_AUTO_BENCHMARK_CASE_TIS(Name, MANAK_DEFAULT_TOLERANCE, Iter, SP)

#define MANAK_AUTO_BENCHMARK_CASE_T(Name, Tol)                                \
MANAK_AUTO_BENCHMARK_CASE_TI(Name, Tol, MANAK_DEFAULT_ITERATIONS)

#define MANAK_AUTO_BENCHMARK_CASE_I(Name, Iter)                               \
MANAK_AUTO_BENCHMARK_CASE_TI(Name, MANAK_DEFAULT_TOLERANCE, Iter)

#define MANAK_AUTO_BENCHMARK_CASE(Name)                                       \
MANAK_AUTO_BENCHMARK_CASE_T(Name, MANAK_DEFAULT_TOLERANCE)

///
/// MANAK_CREATE_BENCHMARK_WITH_TEMPLATE macros
///

#define MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_ITD(Name, f_name, Iter, tol, Desc)      \
( manak::CTManakCase<manak::BenchmarkCase>(#Name, MANAK_STRINGIZE(MANAK_BASE_LIBRARY_NAME), Iter, tol, Desc, f_name) )

#define MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_T(Name, f_name, tol)             \
( manak::CTManakCase<manak::BenchmarkCase>(#Name, MANAK_STRINGIZE(MANAK_BASE_LIBRARY_NAME), MANAK_DEFAULT_ITERATIONS, tol, "", f_name) )

#define MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_I(Name, f_name, Iter)             \
( manak::CTManakCase<manak::BenchmarkCase>(#Name, MANAK_STRINGIZE(MANAK_BASE_LIBRARY_NAME), Iter, MANAK_DEFAULT_TOLERANCE, "", f_name) )

#define MANAK_CREATE_BENCHMARK_WITH_TEMPLATE(Name, f_name)             \
( manak::CTManakCase<manak::BenchmarkCase>(#Name, MANAK_STRINGIZE(MANAK_BASE_LIBRARY_NAME), MANAK_DEFAULT_ITERATIONS, MANAK_DEFAULT_TOLERANCE, "", f_name) )

#define MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_IT(Name, f_name, Iter, Tol)             \
( manak::CTManakCase<manak::BenchmarkCase>(#Name, MANAK_STRINGIZE(MANAK_BASE_LIBRARY_NAME), Iter, Tol, "", f_name) )
