#ifdef MANAK_USE_DYN_LINK
#include <manak/util/macro_utils.hpp>
#include "manak_suite.hpp"
#endif // MANAK_USE_DYN_LINK

namespace manak
{

MANAK_INLINE ManakSuite::~ManakSuite()
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

MANAK_INLINE  ManakSuite* ManakSuite::SetCurrentSuite(const std::string& name)
{
  auto suite = current_manak_suite->child_suits.find(name);
  if(suite != current_manak_suite->child_suits.end())
  {
    current_manak_suite = suite->second;
  }
  else
  {
    ManakSuite* n_s = new ManakSuite(name);
    current_manak_suite->AddSuite(n_s);
    current_manak_suite = n_s;
  }
  return current_manak_suite;
}

MANAK_INLINE ManakCase* ManakSuite::AddCase(ManakCase* obj)
{
  auto it = children.find(obj->Name());
  if(it != children.end())
  {
    for(auto bc : it->second)
    {
      if(bc->LibraryName() == obj->LibraryName())
      {
        std::cerr << "Contains multiple entries for library "
                  << obj->LibraryName() << " with case "
                  << obj->Name() << std::endl;
        exit(1);
      }
    }
    it->second.push_back(obj);
  }
  else
  {
    children[obj->Name()].push_back(obj);
  }
  return obj;
}

MANAK_INLINE bool ManakSuite::LoadData(const std::string& name)
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

MANAK_INLINE bool ManakSuite::Find(const std::string& name, std::list<ManakCase*>& lbc)
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

 MANAK_INLINE bool ManakSuite::Run(const std::string& uname,
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
    it.second->Run(uname + "/" + it.first, pattern, compare);
  }

  ResultCollector::GlobalResultCollector().CloseSuite();

  return true;
}

}
