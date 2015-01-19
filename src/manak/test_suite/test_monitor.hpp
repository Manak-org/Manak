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
  enum class Type {ASSERT, CHECK, WARN};

  void AddAssert(TestEntry* entry)
  {
    entries.emplace_back(Type::ASSERT, entry);
  }

  void AddCheck(TestEntry* entry)
  {
    entries.emplace_back(Type::CHECK, entry);
  }

  void AddWarn(TestEntry* entry)
  {
    entries.emplace_back(Type::WARN, entry);
  }

  std::list<std::tuple<Type, TestEntry*>> entries;
};

struct TestResult
{
  TestResult()
    : new_entry(false) {}

  enum class Res {PASS, FAIL};

  void NewEntry()
  {
    new_entry = true;
  }

  void AddAssert(TestEntry* entry)
  {
    if(new_entry)
    {
      entries.emplace_back(Res::PASS, TestResultEntry());
      new_entry = false;
    }

    std::get<1>(*(--entries.end())).AddAssert(entry);
    std::get<0>(*(--entries.end())) = Res::FAIL;
  }

  void ConfirmEntry()
  {
    if(new_entry)
    {
      entries.emplace_back(Res::PASS, TestResultEntry());
      new_entry = false;
    }
  }

  bool GetStatus(double success_p) const
  {
    for(auto entry : entries)
    {
      if(std::get<0>(entry) == Res::FAIL)
        return false;
    }
    return true;
  }

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

  void Initialize()
  {
    tr = TestResult();
    isTest = false;
    isEnabled = false;
  }

  void NewEntry()
  {
    tr.NewEntry();
  }

  void ConfirmEntry()
  {
    if(isTest)
      tr.ConfirmEntry();
  }

  bool AddAssert(TestEntry* entry)
  {
    if(isEnabled)
    {
      tr.AddAssert(entry);
      return true;
    }
    return false;
  }

  void Enable()
  {
    isEnabled = true;
    isTest = true;
  }

  void Disable()
  {
    isEnabled = false;
  }

  const bool& IsTest() const
  {
    return isTest;
  }

  TestResult Result()
  {
    return tr;
  }

 private:
  TestResult tr;

  bool isEnabled;
  bool isTest;
};

}


#endif // MANAK_TEST_MONITOR_HPP_INCLUDED
