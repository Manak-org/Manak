namespace manak
{

////////////////////////////////////////////////////////////////////////////////
/// TestResultEntry Implementation
////////////////////////////////////////////////////////////////////////////////

TestResultEntry::~TestResultEntry()
{
  for(auto it : entries)
  {
    delete std::get<1>(it);
  }
}

void TestResultEntry::AddAssert(TestEntry* entry)
{
  entries.emplace_back(Type::ASSERT, entry);
}

void TestResultEntry::AddCheck(TestEntry* entry)
{
  entries.emplace_back(Type::CHECK, entry);
}

void TestResultEntry::AddText(TestEntry* entry)
{
  entries.emplace_back(Type::TEXT, entry);
}

void TestResultEntry::AddWarn(TestEntry* entry)
{
  entries.emplace_back(Type::WARN, entry);
}

void TestResultEntry::GetFailMsg(std::list<std::string>& l_str)
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

////////////////////////////////////////////////////////////////////////////////
/// TestResult Implementation
////////////////////////////////////////////////////////////////////////////////

void TestResult::NewEntry()
{
  new_entry = true;
}

void TestResult::AddAssert(TestEntry* entry)
{
  if(new_entry)
  {
    entries.emplace_back(Res::PASS, TestResultEntry());
    new_entry = false;
  }

  std::get<1>(*(--entries.end())).AddAssert(entry);
  std::get<0>(*(--entries.end())) = Res::FAIL;
}

void TestResult::AddText(TestEntry* entry)
{
  if(new_entry)
  {
    entries.emplace_back(Res::PASS, TestResultEntry());
    new_entry = false;
  }

  std::get<1>(*(--entries.end())).AddText(entry);
}

void TestResult::ConfirmEntry()
{
  if(new_entry)
  {
    entries.emplace_back(Res::PASS, TestResultEntry());
    new_entry = false;
  }
}

bool TestResult::GetStatus(double success_p) const
{
  for(auto entry : entries)
  {
    if(std::get<0>(entry) == Res::FAIL)
      return false;
  }
  return true;
}

bool TestResult::GetFailMsg(size_t& n_itr, std::list<std::string>& l_str) const
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

double TestResult::GetSP() const
{
  size_t cfail = 0;
  for(auto entry : entries)
  {
    if(std::get<0>(entry) == Res::FAIL)
      cfail++;
  }
  return (double)(entries.size() - cfail)/ entries.size();
}

////////////////////////////////////////////////////////////////////////////////
/// TestMonitor Implementation
////////////////////////////////////////////////////////////////////////////////

void TestMonitor::Initialize()
{
  tr = TestResult();
  isTest = false;
  isEnabled = false;
}

void TestMonitor::NewEntry()
{
  tr.NewEntry();
}

void TestMonitor::ConfirmEntry()
{
  if(isTest)
    tr.ConfirmEntry();
}

bool TestMonitor::AddAssert(TestEntry* entry)
{
  if(isEnabled)
  {
    tr.AddAssert(entry);
    return true;
  }
  return false;
}

bool TestMonitor::AddText(TestEntry* entry)
{
  if(isEnabled)
  {
    tr.AddText(entry);
    return true;
  }
  return false;
}

void TestMonitor::Enable()
{
  isEnabled = true;
  isTest = true;
}

void TestMonitor::Disable()
{
  isEnabled = false;
}

const bool& TestMonitor::IsTest() const
{
  return isTest;
}

TestResult TestMonitor::Result()
{
  return tr;
}

}
