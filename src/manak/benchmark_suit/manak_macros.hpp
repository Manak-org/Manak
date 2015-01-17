#ifndef MANAK_MACROS_HPP_INCLUDED
#define MANAK_MACROS_HPP_INCLUDED

////////////////////////////////////////////////////////////////////////////////
/// MANAK CASE MACRO
////////////////////////////////////////////////////////////////////////////////

#define _MANAK_CASE_TIS(Type, Name, Library, Function, Tol, Iter, SP)         \
( new manak::Type(#Name, #Library, Function, Tol, Iter, SP) )


////////////////////////////////////////////////////////////////////////////////
/// MANAK BENCHMARK MACRO
////////////////////////////////////////////////////////////////////////////////

#define _MANAK_BENCHMARK_CASE_TIS(Name, Library, Function, Tol, Iter, SP)     \
_MANAK_CASE_TIS(BenchmarkCase, Name, Library, Function, Tol, Iter, SP)

////////////////////////////////////////////////////////////////////////////////
/// MANAK AUTO CASE MACRO
////////////////////////////////////////////////////////////////////////////////

#define _MANAK_AUTO_CASE_TIS(Type, Name, Library, Tol, Iter, SP)              \
struct Name ## _ ## Library                                                   \
{                                                                             \
  static void Run();                                                          \
  static manak::ManakCase* static_temp;                                       \
};                                                                            \
manak::ManakCase* Name ## _ ## Library::static_temp =                         \
manak::ManakSuite::GetMasterSuite().GetCurrentSuite()->                       \
AddCase(_MANAK_CASE_TIS(Type,                                                 \
                        Name,                                                 \
                        Library,                                              \
                        Name ## _ ## Library::Run,                            \
                        Tol,                                                  \
                        Iter,                                                 \
                        SP));                                                 \
void Name ## _ ## Library::Run()

#ifdef MANAK_SIMPLE_MODULE
#include "simple_module_benchmark_case.hpp"

#else // MANAK_SIMPLE_MODULE
#include "module_benchmark_case.hpp"

#endif // MANAK_SIMPLE_MODULE

#define MANAK_ADD_CASE(bench)                                                 \
class STRING_JOIN(unamed, STRING_JOIN(_, __LINE__))                           \
{                                                                             \
  static manak::ManakCase* static_temp;                                       \
};                                                                            \
manak::ManakCase*                                                             \
STRING_JOIN(unamed, STRING_JOIN(_, __LINE__))::static_temp =                  \
manak::ManakSuite::GetMasterSuite().GetCurrentSuite()->AddCase(bench)

#define Measure(Code)                                                         \
manak::Timer::StartTimer();                                                   \
Code                                                                          \
manak::Timer::StopTimer();


#endif // MANAK_MACROS_HPP_INCLUDED
