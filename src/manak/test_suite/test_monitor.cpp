#ifdef MANAK_USE_DYN_LINK
#include "test_monitor.hpp"
#endif // MANAK_USE_DYN_LINK

namespace manak
{

////////////////////////////////////////////////////////////////////////////////
/// TestResultEntry Implementation
////////////////////////////////////////////////////////////////////////////////

MANAK_INLINE TestResultEntry::~TestResultEntry()
{
  for(auto it : entries)
  {
    delete std::get<1>(it);
  }
}

MANAK_INLINE void TestResultEntry::AddAssert(TestEntry* entry)
{
  entries.emplace_back(Type::ASSERT, entry);
}

MANAK_INLINE void TestResultEntry::AddCheck(TestEntry* entry)
{
  entries.emplace_back(Type::CHECK, entry);
}

MANAK_INLINE void TestResultEntry::AddText(TestEntry* entry)
{
  entries.emplace_back(Type::TEXT, entry);
}

MANAK_INLINE void TestResultEntry::AddWarn(TestEntry* entry)
{
  entries.emplace_back(Type::WARN, entry);
}

MANAK_INLINE void TestResultEntry::GetFailMsg(std::list<std::string>& l_str) const
{
  for(auto entry : entries)
  {
    std::stringstream ss;
    ss << std::get<1>(entry)->Filename() << ":" << std::get<1>(entry)->LineNo();
    ss << " : ";

    if(std::get<0>(entry) == Type::ASSERT)
      ss << "Assertion Failed! ";
    else if(std::get<0>(entry) == Type::CHECK)
      ss << "Check Failed! ";

    ss << std::get<1>(entry)->Msg();

    l_str.push_back(ss.str());
  }
}

////////////////////////////////////////////////////////////////////////////////
/// TestResult Implementation
////////////////////////////////////////////////////////////////////////////////

MANAK_INLINE void TestResult::Clear()
{
  entries.clear();
}

MANAK_INLINE void TestResult::NewEntry()
{
  new_entry = true;
}

MANAK_INLINE void TestResult::AddAssert(TestEntry* entry)
{
  if(new_entry)
  {
    entries.emplace_back(Res::PASS, TestResultEntry());
    new_entry = false;
  }

  std::get<1>(*(--entries.end())).AddAssert(entry);
  std::get<0>(*(--entries.end())) = Res::FAIL;
}

MANAK_INLINE void TestResult::AddCheck(TestEntry* entry)
{
  if(new_entry)
  {
    entries.emplace_back(Res::PASS, TestResultEntry());
    new_entry = false;
  }

  std::get<1>(*(--entries.end())).AddCheck(entry);
  std::get<0>(*(--entries.end())) = Res::FAIL;
}

MANAK_INLINE void TestResult::AddText(TestEntry* entry)
{
  if(new_entry)
  {
    entries.emplace_back(Res::PASS, TestResultEntry());
    new_entry = false;
  }

  std::get<1>(*(--entries.end())).AddText(entry);
}

MANAK_INLINE void TestResult::ConfirmEntry()
{
  if(new_entry)
  {
    entries.emplace_back(Res::PASS, TestResultEntry());
    new_entry = false;
  }
}

MANAK_INLINE bool TestResult::GetStatus(double success_p) const
{
  double s_count = 0;
  for(std::list<std::tuple<Res, TestResultEntry>>::const_iterator it = entries.begin();
      it != entries.end();it++)
  {
    if(std::get<0>(*it) == Res::PASS)
      s_count++;
  }

  if(s_count * 100/ entries.size() >= success_p)
    return true;
  else return false;
}

MANAK_INLINE bool TestResult::GetFailMsg(size_t& n_itr, std::list<std::string>& l_str) const
{
  n_itr = 1;
  for(std::list<std::tuple<Res, TestResultEntry>>::const_iterator it = entries.begin();
      it != entries.end();it++)
  {
    if(std::get<0>(*it) == Res::FAIL)
    {
      std::get<1>(*it).GetFailMsg(l_str);
      return true;
    }
  }
  return false;
}

MANAK_INLINE double TestResult::GetSP() const
{
  size_t cfail = 0;
  for(const auto& entry : entries)
  {
    if(std::get<0>(entry) == Res::FAIL)
      cfail++;
  }
  return (double)(entries.size() - cfail)/ entries.size();
}

////////////////////////////////////////////////////////////////////////////////
/// TestMonitor Implementation
////////////////////////////////////////////////////////////////////////////////

MANAK_INLINE void TestMonitor::Initialize()
{
  delete tr;
  tr = new TestResult();
  isTest = false;
  isEnabled = false;
}

MANAK_INLINE void TestMonitor::NewEntry()
{
  tr->NewEntry();
}

MANAK_INLINE void TestMonitor::ConfirmEntry()
{
  if(isTest)
    tr->ConfirmEntry();
}

MANAK_INLINE bool TestMonitor::AddAssert(TestEntry* entry)
{
  if(isEnabled)
  {
    tr->AddAssert(entry);
    return true;
  }
  return false;
}

MANAK_INLINE bool TestMonitor::AddCheck(TestEntry* entry)
{
  if(isEnabled)
  {
    tr->AddCheck(entry);
    return true;
  }
  return false;
}

MANAK_INLINE bool TestMonitor::AddText(TestEntry* entry)
{
  if(isEnabled)
  {
    tr->AddText(entry);
    return true;
  }
  return false;
}

MANAK_INLINE void TestMonitor::Enable()
{
  isEnabled = true;
  isTest = true;
}

MANAK_INLINE void TestMonitor::Disable()
{
  isEnabled = false;
}

MANAK_INLINE const bool& TestMonitor::IsTest() const
{
  return isTest;
}

MANAK_INLINE TestResult*& TestMonitor::Result()
{
  return tr;
}

}
