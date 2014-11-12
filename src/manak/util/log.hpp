/**
 * @file log.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of Log.
 */
#ifndef MANAK_UTIL_LOG_HPP_INCLUDED
#define MANAK_UTIL_LOG_HPP_INCLUDED

#include <map>
#include <list>
#include <string>
#include <iomanip>
#include <sstream>
#include <tuple>

#include <manak/benchmark_suit/pmeasure.hpp>

namespace manak
{
namespace utils
{

/**
 * This struct stores the measures corresponding to one parameter set of a case.
 */
struct LogEntry
{
  //! Create an empty log entry
  LogEntry()
  {
    measures.emplace_back();
  }

  //! Add a measure to this entry.
  void Add(PMeasure pm, size_t index, int inc = 0, const double c_value = 0)
  {
    if(index > measures.size() - 1)
    {
      while(index != measures.size() - 1)
      {
        measures.emplace_back(PMeasure(), false, 0);
      }
      std::get<0>(*(--measures.end())) = pm;
      std::get<1>(*(--measures.end())) = inc;
      std::get<2>(*(--measures.end())) = c_value;
    }
    else
    {
      auto it = measures.begin();
      for(size_t i = 0;i < index;i++)
      {
        it++;
      }
      std::get<0>(*it) = pm;
      std::get<1>(*it) = inc;
      std::get<2>(*it) = c_value;
    }
  }

  //! Print all the measures to the stream in a user friendly way
  void Print(std::ostream& stream)
  {
    for(auto pm : measures)
    {
      std::stringstream s;
      if(std::get<2>(pm) != 0)
      {
        if(std::get<1>(pm) > 0)
          s << "+";
        else if(std::get<2>(pm) < 0)
          s << "-";
        s << std::get<0>(pm);
        s << "(" << std::get<2>(pm) << ")";
      }
      else s << std::get<0>(pm);

      stream << std::setw(20) << s.str();
    }
  }

  //! Print all the measures to the stream
  void Save(std::ostream& stream)
  {
    for(auto pm : measures)
    {
      stream << std::get<0>(pm).avg << " ";
    }
  }

  //! List of entries along with comparison information
  std::list<std::tuple<PMeasure, int, double>> measures;
};

/**
 * This struct stores all the logging information about one benchmark case.
 */
struct CaseLogEntry
{
  //! Create an empty benchmark case log
  CaseLogEntry(const std::string& name, const std::string& uname)
    : name(name), uname(uname)
  {
    entries.emplace_back("", LogEntry());
  }

  //! Add a parameter set to thsi benchmark case
  LogEntry& Add(size_t index, const std::string& name = "")
  {
    if(index > (entries.size() - 1))
    {
      while(index != entries.size() - 1)
      {
        entries.emplace_back("", LogEntry());
      }
      (*(--entries.end())).first = name;
      return (*(--entries.end())).second;
    }
    else
    {
      auto it = entries.begin();
      for(size_t i = 0;i < index;i++)
      {
        it++;
      }
      (*it).first = name;
      return (*it).second;
    }
  }

  //! Print the log in a user friendly way
  void Print(std::ostream& stream)
  {
    if(entries.size() > 1)
    {
      stream << name << std::endl;

      size_t index = 0;
      for(auto le : entries)
      {
        std::stringstream ss;
        ss << "  ";
        if(le.first == "")
          ss << "Parameter Set " << index;
        else
          ss << le.first;

        stream << std::setw(30) << ss.str();
        le.second.Print(stream);
        stream << std::endl;
        index++;
      }
    }
    else
    {
      stream << std::setw(30) << name;
      (*entries.begin()).second.Print(stream);
    }
  }

  //! Print the log
  void Save(std::ostream& stream)
  {
    stream << uname << " " << entries.size() << std::endl;

    for(auto le : entries)
    {
      le.second.Save(stream);
      stream << std::endl;
    }
  }

  //! Name of the case
  std::string name;
  //! Unique name assigned to this case
  std::string uname;
  //! List of entries(parameter set)
  std::list<std::pair<std::string, LogEntry>> entries;
};

/**
 * This class stores and prints all the necessary information about the run.
 */
class Log
{
 public:
  //! Create an empty Log
  Log()
    : c_l_id(0) {}

  //! Get singleton Log instance
  static Log& GetLog()
  {
    static Log singleton;
    return singleton;
  }

  //! Add a benchmark case to his log
  CaseLogEntry& Add(const std::string& name, const std::string& uname)
  {
    cases.emplace_back(name, uname);
    return *(--cases.end());
  }

  //! Add library to this log
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

  //! Print all the information in a user friendly way
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

  //! Save all the information to the file for comparison
  void Save(std::ostream& stream)
  {
    stream << l_ids.size() << " ";

    for(size_t i = 0;i < l_ids.size();i++)
    {
      stream << r_l_ids[i] << " ";
    }
    stream << std::endl;

    stream << cases.size() << std::endl;

    for(auto c : cases)
    {
      c.Save(stream);
    }
  }

 private:
  //! map of library name to id
  std::map<std::string, size_t> l_ids;
  //! map of library id to name
  std::map<size_t, std::string> r_l_ids;
  //! List of benchamrk cases
  std::list<CaseLogEntry> cases;

  //! currently assigned library id.
  size_t c_l_id;
}; // class Log

}; // namespace utils
}; // namespace manak


#endif // MANAK_UTIL_LOG_HPP_INCLUDED
