/**
 * @file module_benchmark_case.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of macros used in MANAK_BENCHMARK_MODULE.
 */

////////////////////////////////////////////////////////////////////////////////
/// MANAK_BENCHMARK_CASE macros
////////////////////////////////////////////////////////////////////////////////

#define MANAK_BENCHMARK_CASE_TIS(Name, Library, Function, Tol, Iter, SP)      \
_MANAK_BENCHMARK_CASE_TIS(Name, Library, Function, Tol, Iter, SP)

#define MANAK_BENCHMARK_CASE_TI(Name, Library, Function, Tol, Iter)           \
MANAK_BENCHMARK_CASE_TIS(Name, Library, Function, Tol, Iter, MANAK_DEFAULT_SP)

#define MANAK_BENCHMARK_CASE_T(Name, Library, Function, Tol)                  \
MANAK_BENCHMARK_CASE_TI(Name, Library, Function, Tol,MANAK_DEFAULT_ITERATIONS)

#define MANAK_BENCHMARK_CASE_I(Name, Library, Function, Iter)                 \
MANAK_BENCHMARK_CASE_TI(Name, Library, Function, MANAK_DEFAULT_TOLERANCE, Iter)

#define MANAK_BENCHMARK_CASE(Name, Library, Function)                         \
MANAK_BENCHMARK_CASE_T(Name, Library, Function, MANAK_DEFAULT_TOLERANCE)



////////////////////////////////////////////////////////////////////////////////
/// MANAK_AUTO_BENCHMARK_CASE macros
////////////////////////////////////////////////////////////////////////////////

#define MANAK_AUTO_BENCHMARK_CASE_TIS(Name, Library, Tol, Iter, SP)           \
_MANAK_AUTO_BENCHMARK_CASE_TIS(Name, Library, Tol, Iter, SP)

#define MANAK_AUTO_BENCHMARK_CASE_TI(Name, Library, Tol, Iter)                \
MANAK_AUTO_BENCHMARK_CASE_TIS(Name, Library, Tol, Iter, MANAK_DEFAULT_SP)

#define MANAK_AUTO_BENCHMARK_CASE_IS(Name, Library, Iter, SP)                 \
MANAK_AUTO_BENCHMARK_CASE_TIS(Name, Library, MANAK_DEFAULT_TOLERANCE, Iter, SP)

#define MANAK_AUTO_BENCHMARK_CASE_T(Name, Library, Tol)                       \
MANAK_AUTO_BENCHMARK_CASE_TI(Name, Library, Tol, MANAK_DEFAULT_ITERATIONS)

#define MANAK_AUTO_BENCHMARK_CASE_I(Name, Library, Iter)                      \
MANAK_AUTO_BENCHMARK_CASE_TI(Name, Library, MANAK_DEFAULT_TOLERANCE, Iter)

#define MANAK_AUTO_BENCHMARK_CASE(Name, Library)                              \
MANAK_AUTO_BENCHMARK_CASE_T(Name, Library, MANAK_DEFAULT_TOLERANCE)

////////////////////////////////////////////////////////////////////////////////
/// MANAK_CREATE_BENCHMARK_WITH_TEMPLATE macros
////////////////////////////////////////////////////////////////////////////////

#define MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_TIS(Name, Lib, Fun, Tol, Iter, SP) \
_MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_TIS(Name, Lib, Fun, Tol, Iter, SP)

#define MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_TI(Name, Lib, Fun, Tol, Iter)    \
MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_TIS(Name, Lib, Fun, Tol, Iter, MANAK_DEFAULT_SP)

#define MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_IS(Name, Lib, Fun, Iter, SP)     \
MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_TIS(Name, Lib, Fun, MANAK_DEFAULT_TOLERANCE, Iter, SP)

#define MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_T(Name, Lib, Fun, Tol)           \
MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_TI(Name, Lib, Fun, Tol, MANAK_DEFAULT_ITERATIONS)

#define MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_I(Name, Lib, Fun, Iter)          \
MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_TI(Name, Lib, Fun, MANAK_DEFAULT_TOLERANCE, Iter)

#define MANAK_CREATE_BENCHMARK_WITH_TEMPLATE(Name, Lib, Fun)                  \
MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_T(Name, Lib, Fun, MANAK_DEFAULT_TOLERANCE)

////////////////////////////////////////////////////////////////////////////////
/// MANAK GROUP BENCHMARK CASE macros
////////////////////////////////////////////////////////////////////////////////

#define MANAK_GROUP_BENCHMARK_CASE_TIS(Name, Lib, Fun, Tol, Iter, SP)         \
_MANAK_GROUP_BENCHMARK_CASE_TIS(Name, Lib, Fun, Tol, Iter, SP)

#define MANAK_GROUP_BENCHMARK_CASE_TI(Name, Lib, Fun, Tol, Iter)              \
MANAK_GROUP_BENCHMARK_CASE_TIS(Name, Lib, Tol, Iter, MANAK_DEFAULT_SP)

#define MANAK_GROUP_BENCHMARK_CASE_IS(Name, Lib, Fun, Iter, SP)               \
MANAK_GROUP_BENCHMARK_CASE_TIS(Name, Lib, MANAK_EFAULT_TOLERANCE, Iter, SP)

#define MANAK_GROUP_BENCHMARK_CASE_T(Name, Lib, Fun, Tol)                     \
MANAK_GROUP_BENCHMARK_CASE_TI(Name, Lib, Tol, MANAK_DEFAULT_ITERATIONS)

#define MANAK_GROUP_BENCHMARK_CASE_I(Name, Lib, Fun, Iter)                    \
MANAK_GROUP_BENCHMARK_CASE_TI(Name, Lib, MANAK_DEFAULT_TOLERANCE, Iter)

#define MANAK_GROUP_BENCHMARK_CASE(Name, Lib, Fun)                            \
MANAK_GROUP_BENCHMARK_CASE_T(Name, Lib, MANAK_DEFAULT_TOLERANCE)

////////////////////////////////////////////////////////////////////////////////
/// MANAK AUTO GROUP BENCHMAK CASE macros
////////////////////////////////////////////////////////////////////////////////

#define MANAK_AUTO_GROUP_BENCHMARK_CASE_TIS(Name, Lib, Tol, Iter, SP)         \
_MANAK_AUTO_GROUP_BENCHMARK_CASE_TIS(Name, Lib, Tol, Iter, SP)

#define MANAK_AUTO_GROUP_BENCHMARK_CASE_TI(Name, Lib, Tol, Iter)              \
MANAK_AUTO_GROUP_BENCHMARK_CASE_TIS(Name, Lib, Tol, Iter, MANAK_DEFAULT_SP)

#define MANAK_AUTO_GROUP_BENCHMARK_CASE_IS(Name, Lib, Iter, SP)               \
MANAK_AUTO_GROUP_BENCHMARK_CASE_TIS(Name, Lib, MANAK_DEFAULT_TOLERANCE, Iter, SP)

#define MANAK_AUTO_GROUP_BENCHMARK_CASE_T(Name, Lib, Tol)                     \
MANAK_AUTO_GROUP_BENCHMARK_CASE_TI(Name, Lib, Tol, MANAK_DEFAULT_ITERATIONS)

#define MANAK_AUTO_GROUP_BENCHMARK_CASE_I(Name, Lib, Iter)                    \
MANAK_AUTO_GROUP_BENCHMARK_CASE_TI(Name, Lib, MANAK_DEFAULT_TOLERANCE, Iter)

#define MANAK_AUTO_GROUP_BENCHMARK_CASE(Name, Lib)                            \
MANAK_AUTO_GROUP_BENCHMARK_CASE_T(Name, Lib, MANAK_DEFAULT_TOLERANCE)
