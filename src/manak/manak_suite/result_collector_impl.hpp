namespace manak
{

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
    }
  }
}

void RCase::Run()
{
  for(auto c : children)
  {
    std::cout << "Running " << c.second->Name() << "...";
    auto l = c.second->Run();
    results[c.first] = l;
    std::cout << " [DONE]" << std::endl;
  }
}

void RCase::Print()
{
  OutputManager::GlobalOutputManager().AddCase(children, results);
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
                              size_t l_id,
                              const std::list<double>& readings)
{
  if(uname == "")
  {
    auto it = results.find(l_id);
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

RNode* RSuite::AddCase(ManakCase* bc, size_t l_id)
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

  return n->AddCase(bc, l_id);
}

RNode* RCase::AddCase(ManakCase* bc, size_t l_id)
{
  children[l_id] = bc;
  return this;
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
                               size_t l_id,
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
      it->second->LoadForComparison(temp, l_id, readings);
    }
  }
}

void ResultCollector::OpenSuite(ManakSuite* suite)
{
  RNode* temp = current_node->AddSuite(suite);
  current_node = temp;
}

void ResultCollector::AddCase(ManakCase* bc)
{
  size_t l_id;
  auto it = l_map.find(bc->LibraryName());
  if(it != l_map.end())
    l_id = it->second;
  else
  {
    l_map[bc->LibraryName()] = current_l_id++;
    l_id = current_l_id - 1;
  }

  current_node->AddCase(bc, l_id);
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

  std::cout << "Running " << total_nodes << " benchmarks with " << l_map.size()
            << " libraries." << std::endl << std::endl;

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

    auto l_it = l_map.find(l_name);
    if(l_it != l_map.end())
    {
      root->LoadForComparison(uname, l_it->second, readings);
    }
  }
}

void ResultCollector::Print()
{
  OutputManager::GlobalOutputManager().Initialize(l_map, isComp, compare_time);

  root->Print();

  OutputManager::GlobalOutputManager().Finalize();
}

}
