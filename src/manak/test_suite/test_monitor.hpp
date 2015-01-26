#ifndef MANAK_TEST_MONITOR_HPP_INCLUDED
#define MANAK_TEST_MONITOR_HPP_INCLUDED

#include <iostream>
#include <string>
#include <list>
#include <tuple>
#include <sstream>

#include "monitor_entry.hpp"

#include <manak/util/macro_utils.hpp>

namespace manak
{

struct TestResultEntry
{
  enum class Type {ASSERT, CHECK, WARN, TEXT};

  MANAK_INLINE ~TestResultEntry();

  MANAK_INLINE void AddAssert(TestEntry* entry);

  MANAK_INLINE void AddCheck(TestEntry* entry);

  MANAK_INLINE void AddText(TestEntry* entry);

  MANAK_INLINE void AddWarn(TestEntry* entry);

  MANAK_INLINE void GetFailMsg(std::list<std::string>& l_str);

  std::list<std::tuple<Type, TestEntry*>> entries;
};

struct TestResult
{
  TestResult()
    : new_entry(false) {}

  enum class Res {PASS, FAIL};

  MANAK_INLINE void NewEntry();

  MANAK_INLINE void AddAssert(TestEntry* entry);

  MANAK_INLINE void AddText(TestEntry* entry);

  MANAK_INLINE void ConfirmEntry();

  MANAK_INLINE bool GetStatus(double success_p) const;

  MANAK_INLINE bool GetFailMsg(size_t& n_itr, std::list<std::string>& l_str) const;

  MANAK_INLINE double GetSP() const;

  std::list<std::tuple<Res, TestResultEntry>> entries;
  bool new_entry;
};

class TestMonitor
{
 public:
  TestMonitor()
    : isEnabled(false),
      isTest(false) {}

  static TestMonitor& GetGlobalTestMonitor()
  {
    static TestMonitor singleton;
    return singleton;
  }

  MANAK_INLINE void Initialize();

  MANAK_INLINE void NewEntry();

  MANAK_INLINE void ConfirmEntry();

  MANAK_INLINE bool AddAssert(TestEntry* entry);

  MANAK_INLINE bool AddText(TestEntry* entry);

  MANAK_INLINE void Enable();

  MANAK_INLINE void Disable();

  MANAK_INLINE const bool& IsTest() const;

  MANAK_INLINE TestResult Result();

 private:
  TestResult tr;

  bool isEnabled;
  bool isTest;
};

}

#ifndef MANAK_USE_DYN_LINK
#include "test_monitor.cpp"
#endif // MANAK_USE_DYN_LINK

#endif // MANAK_TEST_MONITOR_HPP_INCLUDED
