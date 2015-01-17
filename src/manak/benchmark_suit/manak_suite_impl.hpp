namespace manak
{

ManakSuite::~ManakSuite()
{
  for(auto lbc : children)
  {
    for(auto bc : lbc.second)
    {
      delete bc;
    }
  }
  for(auto bs : child_suits)
  {
    delete bs.second;
  }
}

bool ManakSuite::LoadData(const std::string& name)
{
  std::ifstream stream(name);

  if(!stream.is_open())
  {
    std::cerr << "Unable to open file " << name << " for comparison" << std::endl;
    return false;
  }

  std::vector<std::string> libs;

  size_t s_libs;

  stream >> s_libs;
  for(size_t i = 0;i < s_libs;i++)
  {
    std::string temp;
    stream >> temp;
    libs.push_back(temp);
  }

  size_t s_cases;
  stream >> s_cases;
  for(size_t i = 0;i < s_cases;i++)
  {
    std::string name;
    stream >> name;
    size_t s_entries;
    stream >> s_entries;

    std::list<ManakCase*> lbc;
    if(Find(name, lbc))
    {
      for(size_t i = 0;i < s_entries;i++)
      {
        size_t s_measures;
        stream >> s_measures;

        for(size_t i = 0;i < s_measures;i++)
        {
          double c_value;
          stream >> c_value;

          for(auto bc : lbc)
          {
            if(bc->LibraryName() == libs[i])
            {
              bc->AddComparisonEntry(c_value);
              break;
            }
          }
        }
      }
    }
  }
  return true;
}

bool ManakSuite::Find(const std::string& name, std::list<ManakCase*>& lbc)
{
  size_t t = name.find("/", 1);
  if(t == std::string::npos)
  {
    auto it = children.find(name.substr(1));
    if(it != children.end())
    {
      lbc = it->second;
      return true;
    }
    else return false;
  }
  else
  {
    auto it = child_suits.find(name.substr(1, t - 1));
    if(it != child_suits.end())
    {
      return it->second->Find(name.substr(t), lbc);
    }
    else return false;
  }
}

 bool ManakSuite::Run(const std::string& uname,
                      const std::string& pattern,
                      const bool compare)
{
  this->uname = uname + "/" + name;

  ResultCollector::GlobalResultCollector().OpenSuite(this);

  std::regex r1(pattern);

  for(auto cases : children)
  {
    std::string match_string = "";
    if(uname == "")
      match_string += cases.first;
    else match_string = uname + "/" + cases.first;

    if((pattern == "") || std::regex_match(match_string, r1))
    {
      for(auto c : cases.second)
      {
        c->UName() = uname + "/" + cases.first;
        ResultCollector::GlobalResultCollector().AddCase(c);
      }
    }
  }

  for(auto it : child_suits)
  {
    it.second->Run(uname + "/" + name, pattern, compare);
  }

  ResultCollector::GlobalResultCollector().CloseSuite();

  return true;
}

}
