#ifndef MANAK_TEST_MACROS_HPP_INCLUDED
#define MANAK_TEST_MACROS_HPP_INCLUDED

#include "test_monitor.hpp"
#include "monitor_entry.hpp"
#include <exception>
#include <string>

#define Test(Code)                                                            \
manak::TestMonitor::GetGlobalTestMonitor().Enable();                          \
try                                                                           \
{                                                                             \
  Code;                                                                       \
} catch(std::exception& ex)                                                   \
{                                                                             \
}

#define MANAK_ASSERT_TRUE(expr)                                               \
if(expr);                                                                     \
else                                                                          \
{                                                                             \
  if(manak::TestMonitor::GetGlobalTestMonitor().AddAssert(new manak::TrueTestEntry(MANAK_STRINGIZE(__FILE__), __LINE__))); \
}

#endif // MANAK_TEST_MACROS_HPP_INCLUDED
