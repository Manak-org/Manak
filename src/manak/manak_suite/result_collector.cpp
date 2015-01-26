#ifdef MANAK_USE_DYN_LINK
#include <manak/util/macro_utils.hpp>
#include <manak/util/manak_env.hpp>
#include "manak_suite.hpp"

#include "result_collector.hpp"
#endif // MANAK_USE_DYN_LINK

namespace manak
{

////////////////////////////////////////////////////////////////////////////////
/// RCASE IMPLEMENTATION
////////////////////////////////////////////////////////////////////////////////

MANAK_INLINE RCase::~RCase()
{
  for(auto it : results)
  {
    for(auto it2 : it.second)
    {
      std::string* name = (std::string*)it2.Get("name");
      delete name;
      double* tol = (double*)it2.Get("tolerance");
      delete tol;
      PMeasure* pm = (PMeasure*)it2.Get("pmeasure");
      delete pm;
      double* com = (double*)it2.Get("compare");
      delete com;
      size_t* iter = (size_t*)it2.Get("iterations");
      delete iter;
      double* sp = (double*)it2.Get("sp");
      delete sp;
      bool* is_test = (bool*)it2.Get("is_test");
      if(*is_test)
      {
        TestResult* tr = (TestResult*)it2.Get("test_res");
        delete tr;
      }
      delete is_test;
    }
  }
}

MANAK_INLINE void RCase::Run()
{
  std::cout << "Running case " << children.begin()->second->UName() << "...";
  for(auto c : children)
  {
    auto l = c.second->Run();
    results[c.first] = l;
  }
  std::cout << " [DONE]" << std::endl;
}

MANAK_INLINE void RCase::Print()
{
  OutputManager::GlobalOutputManager().AddCase(children.begin()->second->UName(),
                                               children.begin()->second->Name(),
                                               results);
}

MANAK_INLINE void RCase::SaveForComparison(std::ostream& stream)
{
  for(auto it : children)
  {
    stream << it.second->UName() << " " << it.second->LibraryName();
    auto result = results.find(it.first);
    stream << " " << result->second.size();
    for(auto res : result->second)
    {
      stream << " " << ((PMeasure*)res.Get("pmeasure"))->avg;
    }
    stream << std::endl;
  }
}

MANAK_INLINE void RCase::LoadForComparison(const std::string& uname,
                                           std::string lname,
                                           const std::list<double>& readings)
{
  if(uname == "")
  {
    auto it = results.find(lname);
    if(it != results.end())
    {
      auto r_it = readings.begin();
      for(auto& l_it : it->second)
      {
        if(r_it != readings.end())
        {
          *(double*)l_it.Get("compare") = *r_it;
        }
        else break;
      }
    }
  }
}

MANAK_INLINE RNode* RCase::AddCase(ManakCase* bc)
{
  children[bc->LibraryName()] = bc;
  return this;
}

////////////////////////////////////////////////////////////////////////////////
/// RSUITE IMPLEMENTATION
////////////////////////////////////////////////////////////////////////////////

MANAK_INLINE RSuite::~RSuite()
{
  for(auto it : next_cases)
  {
    delete it.second;
  }

  for(auto it : next_suites)
  {
    delete it.second;
  }
}

MANAK_INLINE RNode* RSuite::AddSuite(ManakSuite* suite)
{
  RNode* n;
  auto it = next_suites.find(suite->Name());
  if(it != next_suites.end())
  {
    n = it->second;
    return n;
  }
  n = new RSuite(this, suite);
  next_suites[suite->Name()] = n;
  return n;
}

MANAK_INLINE RNode* RSuite::EraseSuite(ManakSuite* suite)
{
  auto it = next_suites.find(suite->Name());
  if(it != next_suites.end())
  {
    RNode* out = it->second;
    next_suites.erase(it);
    return out;
  }
  return NULL;
}

MANAK_INLINE RNode* RSuite::AddCase(ManakCase* bc)
{
  RNode* n;
  auto it = next_cases.find(bc->Name());
  if(it != next_cases.end())
  {
    n = it->second;
  }
  else
  {
    n = new RCase(this);
    next_cases[bc->Name()] = n;
  }

  return n->AddCase(bc);
}

MANAK_INLINE void RSuite::Run()
{
  for(auto n : next_cases)
  {
    n.second->Run();
  }

  for(auto n : next_suites)
  {
    n.second->Run();
  }
}

MANAK_INLINE void RSuite::Print()
{
  for(auto n : next_cases)
  {
    n.second->Print();
  }

  for(auto n : next_suites)
  {
    n.second->Print();
  }
}

MANAK_INLINE void RSuite::SaveForComparison(std::ostream& stream)
{
  for(auto it : next_cases)
  {
    it.second->SaveForComparison(stream);
  }

  for(auto it : next_suites)
  {
    it.second->SaveForComparison(stream);
  }
}

MANAK_INLINE void RSuite::LoadForComparison(const std::string& uname,
                                            std::string lname,
                                            const std::list<double>& readings)
{
  if(uname != "")
  {
    size_t index = uname.find("/");

    std::string temp = "";
    std::string c_name = "";

    if(index != std::string::npos)
    {
      temp = uname.substr(index + 1, uname.size());
      c_name = uname.substr(0, index);

      auto it = next_suites.find(c_name);
      if(it != next_suites.end())
      {
        it->second->LoadForComparison(temp, lname, readings);
      }
    }
    else
    {
      c_name = uname;

      auto it = next_cases.find(c_name);
      if(it != next_cases.end())
      {
        it->second->LoadForComparison(temp, lname, readings);
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
/// RESULT COLLECTOR IMPLEMENTATION
////////////////////////////////////////////////////////////////////////////////

MANAK_INLINE void ResultCollector::OpenSuite(ManakSuite* suite)
{
  RNode* temp = current_node->AddSuite(suite);
  current_node = temp;
}

MANAK_INLINE void ResultCollector::AddCase(ManakCase* bc)
{
  current_node->AddCase(bc);
  current_node->count++;
}

MANAK_INLINE void ResultCollector::CloseSuite()
{
  if(current_node->count == 0)
  {
    current_node->parent->EraseSuite(current_node->GetSuite());
    RNode* temp = current_node;
    current_node = current_node->parent;
    delete temp;
    return;
  }

  current_node->parent->count++;
  current_node = current_node->parent;
}

MANAK_INLINE void ResultCollector::Run()
{
  std::cout << std::setiosflags(std::ios::left);
  std::cout << "######################################################################"
            << std::endl;
  std::cout << "#  " << std::setw(66) << "Manak C++ Benchmarking Library" << "#"
            << std::endl;

  std::stringstream ss;
  ss << "Version " << __MANAK_VERSION_MAJOR << "." << __MANAK_VERSION_MINOR
     << "." << __MANAK_VERSION_PATCH;
  std::cout << "#  " << std::setw(66) << ss.str() << "#" << std::endl;

  std::cout << "######################################################################"
            << std::endl << std::endl;

  root->Run();
}

MANAK_INLINE void ResultCollector::SaveForComparison(std::ostream& stream)
{
  stream << GetVersionInfo() << std::endl;

  stream << Timer::getTimeStamp() << std::endl;

  root->SaveForComparison(stream);
}

MANAK_INLINE void ResultCollector::LoadForComparison(std::istream& stream)
{
  isComp = true;

  std::string temp;

  getline(stream, temp);
  getline(stream, temp);
  getline(stream, temp);

  //! extract time
  getline(stream, temp);
  compare_time = temp;

  //! get all the cases
  while(getline(stream, temp))
  {
    std::stringstream ss;
    ss << temp;
    std::string uname;
    ss >> uname;

    std::string l_name;
    ss >> l_name;

    size_t num_readings = 0;
    ss >> num_readings;

    std::list<double> readings;

    for(size_t i = 0;i < num_readings;i++)
    {
      double temp;
      ss >> temp;
      readings.push_back(temp);
    }

    uname = uname.substr(1, uname.length() - 1);

    root->LoadForComparison(ManakEnv::GlobalEnv().GetModuleName() + "/" + uname, l_name, readings);
  }
}

MANAK_INLINE void ResultCollector::Print()
{
  OutputManager::GlobalOutputManager().Initialize(isComp, compare_time);

  root->Print();

  OutputManager::GlobalOutputManager().Finalize();
}

}
