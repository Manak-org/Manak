#ifndef MANAK_MACROS_HPP_INCLUDED
#define MANAK_MACROS_HPP_INCLUDED

////////////////////////////////////////////////////////////////////////////////
/// MANAK CASE MACROS
////////////////////////////////////////////////////////////////////////////////

#define MANAK_ADD_CASE(bench)                                                 \
class STRING_JOIN(unamed, STRING_JOIN(_, __LINE__))                           \
{                                                                             \
  static manak::ManakCase* static_temp;                                       \
};                                                                            \
manak::ManakCase*                                                             \
STRING_JOIN(unamed, STRING_JOIN(_, __LINE__))::static_temp =                  \
manak::ManakSuite::GetMasterSuite().GetCurrentSuite()->AddCase(bench)

#define _MANAK_CASE_TIS(Type, Name, Library, Function, Tol, Iter, SP)         \
( new manak::Type(#Name, #Library, Function, Tol, Iter, SP) )

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

////////////////////////////////////////////////////////////////////////////////
/// MANAK CREATE BENCHMARK WITH TEMPLATE MACRO
////////////////////////////////////////////////////////////////////////////////

#define _MANAK_CREATE_WITH_TEMPLATE_TIS(Type, Name, Library, Function, Tol, Iter, SP)  \
( manak::CTManakCase<manak::Type>(#Name, MANAK_STRINGIZE(Library), Function, Tol, Iter, SP) )

#endif // MANAK_MACROS_HPP_INCLUDED
