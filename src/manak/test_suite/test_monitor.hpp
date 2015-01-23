#ifndef MANAK_TEST_MONITOR_HPP_INCLUDED
#define MANAK_TEST_MONITOR_HPP_INCLUDED

#include <iostream>
#include <string>
#include <list>
#include <tuple>

#include "monitor_entry.hpp"

namespace manak
{

struct TestResultEntry
{
  enum class Type {ASSERT, CHECK, WARN, TEXT};

  ~TestResultEntry();

  void AddAssert(TestEntry* entry);

  void AddCheck(TestEntry* entry);

  void AddText(TestEntry* entry);

  void AddWarn(TestEntry* entry);

  void GetFailMsg(std::list<std::string>& l_str);

  std::list<std::tuple<Type, TestEntry*>> entries;
};

struct TestResult
{
  TestResult()
    : new_entry(false) {}

  enum class Res {PASS, FAIL};

  void NewEntry();

  void AddAssert(TestEntry* entry);

  void AddText(TestEntry* entry);

  void ConfirmEntry();

  bool GetStatus(double success_p) const;

  bool GetFailMsg(size_t& n_itr, std::list<std::string>& l_str) const;

  double GetSP() const;

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

  void Initialize();

  void NewEntry();

  void ConfirmEntry();

  bool AddAssert(TestEntry* entry);

  bool AddText(TestEntry* entry);

  void Enable();

  void Disable();

  const bool& IsTest() const;

  TestResult Result();

 private:
  TestResult tr;

  bool isEnabled;
  bool isTest;
};

}

#include "test_monitor_impl.hpp"

#endif // MANAK_TEST_MONITOR_HPP_INCLUDED
