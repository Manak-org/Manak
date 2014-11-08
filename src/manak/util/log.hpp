#ifndef MANAK_UTIL_LOG_HPP_INCLUDED
#define MANAK_UTIL_LOG_HPP_INCLUDED

#include <map>
#include <list>
#include <string>
#include <iomanip>

#include <manak/benchmark_suit/pmeasure.hpp>

namespace manak
{
namespace utils
{

struct LogEntry
{
  LogEntry()
  {
    measures.emplace_back();
  }

  void Add(PMeasure pm, size_t index)
  {
    if(index > measures.size() - 1)
    {
      while(index != measures.size() - 1)
      {
        measures.emplace_back();
      }
      *(--measures.end()) = pm;
    }
    else
    {
      auto it = measures.begin();
      for(size_t i = 0;i < index;i++)
      {
        it++;
      }
      *it = pm;
    }
  }

  void Print(std::ostream& stream)
  {
    for(auto pm : measures)
    {
      stream << std::setw(20) << pm;
    }
  }

  std::list<PMeasure> measures;
};

struct CaseLogEntry
{
  CaseLogEntry(const std::string& name)
    : name(name)
  {
    entries.emplace_back();
  }

  LogEntry& Add(size_t index)
  {
    if(index > (entries.size() - 1))
    {
      while(index != entries.size() - 1)
      {
        entries.emplace_back();
      }
      return *(--entries.end());
    }
    else
    {
      auto it = entries.begin();
      for(size_t i = 0;i < index;i++)
      {
        it++;
      }
      return *it;
    }
  }

  void Print(std::ostream& stream)
  {
    stream << name << std::endl;

    size_t index = 0;
    for(auto le : entries)
    {
      stream << "  Parameter Set " << std::setw(14) << index;
      le.Print(stream);
      stream << std::endl;
      index++;
    }
  }

  std::string name;
  std::list<LogEntry> entries;
};

class Log
{
 public:
  Log()
    : c_l_id(0) {}

  static Log& GetLog()
  {
    static Log singleton;
    return singleton;
  }

  CaseLogEntry& Add(const std::string& name)
  {
    cases.emplace_back(name);
    return *(--cases.end());
  }

  size_t AddLibrary(const std::string& name)
  {
    auto it = l_ids.find(name);
    if(it == l_ids.end())
    {
      l_ids[name] = c_l_id++;
      r_l_ids[c_l_id - 1] = name;
      return c_l_id - 1;
    }

    return it->second;
  }

  void Print(std::ostream& stream)
  {
    stream << std::setprecision(3);
    stream << std::setiosflags(std::ios::left) << std::setw(30) << "       Case Name";
    for(size_t i = 0;i < l_ids.size();i++)
    {
      stream << std::setw(20) << r_l_ids[i];
    }
    stream << std::endl << std::endl;

    for(auto c : cases)
    {
      c.Print(stream);
      stream << std::endl;
    }
  }

 private:
  std::map<std::string, size_t> l_ids;
  std::map<size_t, std::string> r_l_ids;
  std::list<CaseLogEntry> cases;

  size_t c_l_id;
};

}
}


#endif // MANAK_UTIL_LOG_HPP_INCLUDED
