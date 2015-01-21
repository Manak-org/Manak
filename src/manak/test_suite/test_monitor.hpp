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

  void AddAssert(TestEntry* entry)
  {
    entries.emplace_back(Type::ASSERT, entry);
  }

  void AddCheck(TestEntry* entry)
  {
    entries.emplace_back(Type::CHECK, entry);
  }

  void AddText(TestEntry* entry)
  {
    entries.emplace_back(Type::TEXT, entry);
  }

  void AddWarn(TestEntry* entry)
  {
    entries.emplace_back(Type::WARN, entry);
  }

  void GetFailMsg(std::list<std::string>& l_str)
  {
    for(auto entry : entries)
    {
      std::stringstream ss;
      ss << std::get<1>(entry)->Filename() << ":" << std::get<1>(entry)->LineNo();
      ss << " : ";

      if(std::get<0>(entry) == Type::ASSERT)
        ss << "Assertion Failed! ";

      ss << std::get<1>(entry)->Msg();

      l_str.push_back(ss.str());
    }
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

  void AddText(TestEntry* entry)
  {
    if(new_entry)
    {
      entries.emplace_back(Res::PASS, TestResultEntry());
      new_entry = false;
    }

    std::get<1>(*(--entries.end())).AddText(entry);
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

  bool GetFailMsg(size_t& n_itr, std::list<std::string>& l_str) const
  {
    n_itr = 1;
    for(auto res : entries)
    {
      if(std::get<0>(res) == Res::FAIL)
      {
        std::get<1>(res).GetFailMsg(l_str);
        return true;
      }
    }
    return false;
  }

  double GetSP() const
  {
    size_t cfail = 0;
    for(auto entry : entries)
    {
      if(std::get<0>(entry) == Res::FAIL)
        cfail++;
    }
    return (double)(entries.size() - cfail)/ entries.size();
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

  bool AddText(TestEntry* entry)
  {
    if(isEnabled)
    {
      tr.AddText(entry);
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
