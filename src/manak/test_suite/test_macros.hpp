#ifndef MANAK_TEST_MACROS_HPP_INCLUDED
#define MANAK_TEST_MACROS_HPP_INCLUDED

#include "test_monitor.hpp"
#include "monitor_entry.hpp"
#include <exception>
#include <string>

#include "test_monitor.hpp"

namespace manak
{

struct ManakException : public std::exception
{};

}

#define Test(Code)                                                            \
manak::TestMonitor::GetGlobalTestMonitor().Enable();                          \
try                                                                           \
{                                                                             \
  Code;                                                                       \
} catch(manak::ManakException& ex){}                                          \
catch(std::exception& ex)                                                     \
{                                                                             \
  manak::TestMonitor::GetGlobalTestMonitor().AddText(new manak::MsgEntry(MANAK_STRINGIZE(__FILE__), __LINE__, ex.what())); \
}

#define MANAK_ASSERT_TRUE(expr)                                               \
if(expr);                                                                     \
else                                                                          \
{                                                                             \
  if(manak::TestMonitor::GetGlobalTestMonitor().AddAssert(new manak::TrueTestEntry(MANAK_STRINGIZE(__FILE__), __LINE__))) \
    throw manak::ManakException();                                            \
}

#define MANAK_ASSERT_TRUE_MSG(expr, msg)                                      \
if(expr);                                                                     \
else                                                                          \
{                                                                             \
  if(manak::TestMonitor::GetGlobalTestMonitor().AddAssert(new manak::MsgEntry(MANAK_STRINGIZE(__FILE__), __LINE__, msg))) \
    throw manak::ManakException();                                            \
}

#endif // MANAK_TEST_MACROS_HPP_INCLUDED
