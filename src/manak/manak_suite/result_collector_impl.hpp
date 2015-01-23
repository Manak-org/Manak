namespace manak
{

////////////////////////////////////////////////////////////////////////////////
/// RCASE IMPLEMENTATION
////////////////////////////////////////////////////////////////////////////////

RCase::~RCase()
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

void RCase::Run()
{
  std::cout << "Running case " << children.begin()->second->Name() << "...";
  for(auto c : children)
  {
    auto l = c.second->Run();
    results[c.first] = l;
  }
  std::cout << " [DONE]" << std::endl;
}

void RCase::Print()
{
  OutputManager::GlobalOutputManager().AddCase(children.begin()->second->UName(),
                                               children.begin()->second->Name(),
                                               results);
}

void RCase::SaveForComparison(std::ostream& stream)
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

void RCase::LoadForComparison(const std::string& uname,
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

RNode* RCase::AddCase(ManakCase* bc)
{
  children[bc->LibraryName()] = bc;
  return this;
}

////////////////////////////////////////////////////////////////////////////////
/// RSUITE IMPLEMENTATION
////////////////////////////////////////////////////////////////////////////////

RSuite::~RSuite()
{
  for(auto it : nexts)
  {
    delete it.second;
  }
}

RNode* RSuite::AddSuite(ManakSuite* suite)
{
  RNode* n;
  auto it = nexts.find(suite->Name());
  if(it != nexts.end())
  {
    n = it->second;
    return n;
  }
  n = new RSuite(this, suite);
  nexts[suite->Name()] = n;
  return n;
}

RNode* RSuite::EraseSuite(ManakSuite* suite)
{
  auto it = nexts.find(suite->Name());
  if(it != nexts.end())
  {
    RNode* out = it->second;
    nexts.erase(it);
    return out;
  }
  return NULL;
}

RNode* RSuite::AddCase(ManakCase* bc)
{
  RNode* n;
  auto it = nexts.find(bc->Name());
  if(it != nexts.end())
  {
    n = it->second;
  }
  else
  {
    n = new RCase(this);
    nexts[bc->Name()] = n;
  }

  return n->AddCase(bc);
}

void RSuite::Run()
{
  for(auto n : nexts)
  {
    n.second->Run();
  }
}

void RSuite::Print()
{
  for(auto n : nexts)
  {
    n.second->Print();
  }
}

void RSuite::SaveForComparison(std::ostream& stream)
{
  for(auto it : nexts)
  {
    it.second->SaveForComparison(stream);
  }
}

void RSuite::LoadForComparison(const std::string& uname,
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
    }
    else
      c_name = uname;

    auto it = nexts.find(c_name);
    if(it != nexts.end())
    {
      it->second->LoadForComparison(temp, lname, readings);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
/// RESULT COLLECTOR IMPLEMENTATION
////////////////////////////////////////////////////////////////////////////////

void ResultCollector::OpenSuite(ManakSuite* suite)
{
  RNode* temp = current_node->AddSuite(suite);
  current_node = temp;
}

void ResultCollector::AddCase(ManakCase* bc)
{
  current_node->AddCase(bc);
  current_node->count++;
}

void ResultCollector::CloseSuite()
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

void ResultCollector::Run()
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

void ResultCollector::SaveForComparison(std::ostream& stream)
{
  stream << GetVersionInfo() << std::endl;

  stream << Timer::getTimeStamp() << std::endl;

  root->SaveForComparison(stream);
}

void ResultCollector::LoadForComparison(std::istream& stream)
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

    root->LoadForComparison(MANAK_MODULE_NAME + ("/" + uname), l_name, readings);
  }
}

void ResultCollector::Print()
{
  OutputManager::GlobalOutputManager().Initialize(isComp, compare_time);

  root->Print();

  OutputManager::GlobalOutputManager().Finalize();
}

}
