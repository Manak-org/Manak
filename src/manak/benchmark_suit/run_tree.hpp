#ifndef _MANAK_RUN_TREE_HPP_INCLUDED
#define _MANAK_RUN_TREE_HPP_INCLUDED

#include <map>
#include <string>
#include <list>
#include <tuple>

#include "pmeasure.hpp"
#include "benchmark_case.hpp"

#include <manak/util/version.hpp>
#include <manak/util/macro_utils.hpp>

namespace manak
{

struct RNode
{
  RNode(RNode* parent)
    : parent(parent)
  {

  }

  bool AddNext(const std::string& name,
               RNode*& n)
  {
    auto it = nexts.find(name);
    if(it != nexts.end())
    {
      n = it->second;
      return false;
    }
    n = new RNode(this);
    nexts[name] = n;
    return true;
  }

  void AddCase(BenchmarkCase* bc, size_t l_id)
  {
    children[l_id] = bc;
  }

  std::string GetPMRep(const std::tuple<std::string, double, PMeasure, double>& entry)
  {
    std::stringstream ss;

    double comp_val = std::get<3>(entry);
    PMeasure pm = std::get<2>(entry);

    if(comp_val >= 0)
    {
      double tol = std::get<1>(entry);

      int res = pm.Compare(comp_val, tol);

      if(res > 0)
        ss << "+";
      else if(res < 0)
        ss << "-";
      ss << std::get<2>(entry) << "(" << comp_val << ")";
    }
    else
      ss << std::get<2>(entry);
    return ss.str();
  }

  void Run()
  {
    for(auto n : nexts)
    {
      n.second->Run();
    }

    for(auto c : children)
    {
      std::cout << "Running " << c.second->Name() << "...";
      auto l = c.second->Run();
      results[c.first] = l;
      std::cout << " [DONE]" << std::endl;
    }
  }

  void PrintTXT(std::ostream& stream, size_t l_ids)
  {
    for(auto it : nexts)
    {
      it.second->PrintTXT(stream, l_ids);
    }

    if(children.size() != 0)
    {
      std::list<std::tuple<std::string, double, PMeasure, double>> dummy;
      std::list<std::tuple<std::string, double, PMeasure, double>>::iterator it_s[l_ids];

      for(size_t i = 0;i < l_ids;i++)
      {
        auto it = results.find(i);
        if(it != results.end())
          it_s[i] = it->second.begin();
        else it_s[i] = dummy.end();
      }

      size_t num_entries = 0;
      for(auto l : results)
      {
        if(l.second.size() > num_entries)
          num_entries = l.second.size();
      }

      if(num_entries > 1)
      {
        stream << children.begin()->second->Name() << std::endl;

        for(size_t index = 0;index < num_entries;index++)
        {
          std::stringstream s_values;
          s_values << std::setiosflags(std::ios::left);

          std::string sub_name = "";
          bool name_mismatch = false;

          for(size_t i = 0;i < l_ids;i++)
          {
            std::stringstream ss;

            if(it_s[i] != dummy.end())
            {
              if(it_s[i] != results.find(i)->second.end())
              {
                if(!name_mismatch)
                {
                  if(sub_name != "" && sub_name != std::get<0>(*it_s[i]))
                    name_mismatch = true;
                  else
                    sub_name = std::get<0>(*it_s[i]);
                }

                ss << GetPMRep(*it_s[i]);

                it_s[i]++;
              }
              else ss << "---";
            }
            else ss << "---";

            s_values << std::setw(20) << ss.str();
          }

          std::stringstream s_sub_name;
          s_sub_name << "  ";
          if(!name_mismatch)
          {
            s_sub_name << sub_name;
          }
          else
          {
            s_sub_name << "Parameter Set " << index;
          }

          stream << std::setw(30) << s_sub_name.str();
          stream << s_values.str();

          stream << std::endl;
        }
      }
      else
      {
        stream << std::setw(30) << children.begin()->second->Name();

        for(size_t i = 0;i < l_ids;i++)
        {
          std::stringstream ss;

          if(it_s[i] != dummy.end())
          {
            if(it_s[i] != results.find(i)->second.end())
            {
              ss << GetPMRep(*it_s[i]);
            }
            else ss << "---";
          }
          else ss << "---";

          stream << std::setw(20) << ss.str();
        }

        stream << std::endl;
      }
      stream << std::endl;
    }
  }

  void PrintHTML(std::ostream& stream, size_t l_ids)
  {
    for(auto it : nexts)
    {
      it.second->PrintHTML(stream, l_ids);
    }

    if(children.size() != 0)
    {
      std::list<std::tuple<std::string, double, PMeasure, double>> dummy;
      std::list<std::tuple<std::string, double, PMeasure, double>>::iterator it_s[l_ids];

      for(size_t i = 0;i < l_ids;i++)
      {
        auto it = results.find(i);
        if(it != results.end())
          it_s[i] = it->second.begin();
        else it_s[i] = dummy.end();
      }

      size_t num_entries = 0;
      for(auto l : results)
      {
        if(l.second.size() > num_entries)
          num_entries = l.second.size();
      }


      if(num_entries > 1)
      {
        stream << "<tr>" << std::endl;
        stream << "<td rowspan = \"" << num_entries
               << "\">" << children.begin()->second->Name()
               << "</td>" << std::endl;

        for(size_t index = 0;index < num_entries;index++)
        {
          std::stringstream s_values;

          std::string sub_name = "";
          bool name_mismatch = false;

          for(size_t i = 0;i < l_ids;i++)
          {
            std::stringstream ss;

            if(it_s[i] != dummy.end())
            {
              if(it_s[i] != results.find(i)->second.end())
              {
                if(!name_mismatch)
                {
                  if(sub_name != "" && sub_name != std::get<0>(*it_s[i]))
                    name_mismatch = true;
                  else
                    sub_name = std::get<0>(*it_s[i]);
                }

                ss << GetPMRep(*it_s[i]);

                it_s[i]++;
              }
              else ss << "---";
            }
            else ss << "---";

            s_values << "<td>" << ss.str() << "</td>" << std::endl;
          }

          std::stringstream s_sub_name;
          if(!name_mismatch)
          {
            s_sub_name << sub_name;
          }
          else
          {
            s_sub_name << "Parameter Set " << index;
          }

          stream << "<td>" << s_sub_name.str() << "</td>" << std::endl;
          stream << s_values.str() << std::endl;
          stream << "</tr>" << std::endl;
          if(index != num_entries - 1)
            stream << "<tr>" << std::endl;
        }
      }
      else
      {
        stream << "<tr>" << std::endl;
        stream << "<td>" << children.begin()->second->Name() << "</td>" << std::endl;
        stream << "<td></td>" << std::endl;
        for(size_t i = 0;i < l_ids;i++)
        {
          std::stringstream ss;

          if(it_s[i] != dummy.end())
          {
            if(it_s[i] != results.find(i)->second.end())
            {
              ss << GetPMRep(*it_s[i]);
            }
            else ss << "---";
          }
          else ss << "---";

          stream << "<td>" << ss.str() << "</td>" << std::endl;
        }
        stream << "</tr>" << std::endl;
      }
    }
  }

  void SaveForComparison(std::ostream& stream, const std::string& uname)
  {
    for(auto it : nexts)
    {
      it.second->SaveForComparison(stream, uname + "/" + it.first);
    }

    if(children.size() != 0)
    {
      for(auto it : children)
      {
        stream << uname << " " << it.second->LibraryName();
        auto result = results.find(it.first);
        stream << " " << result->second.size();
        for(auto res : result->second)
        {
          stream << " " << std::get<2>(res).avg;
        }
        stream << std::endl;
      }
    }
  }

  void LoadForComparison(const std::string& uname,
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
    else if(children.size() != 0)
    {
      auto it = results.find(l_id);
      if(it != results.end())
      {
        auto r_it = readings.begin();
        for(auto& l_it : it->second)
        {
          if(r_it != readings.end())
          {
            std::get<3>(l_it) = *r_it;
          }
          else break;
        }
      }
    }
  }

  RNode* parent;

  std::map<std::string, RNode*> nexts;

  std::map<size_t, BenchmarkCase*> children;
  std::map<size_t, std::list<std::tuple<std::string, double, PMeasure, double>>> results;
};

class RunTree
{
 public:
  RunTree()
    : root(new RNode(NULL)),
    current_node(root),
    total_nodes(0),
    current_l_id(0)
  {
  }

  static RunTree& GlobalRunTree()
  {
    static RunTree singleton;
    return singleton;
  }

  void AddSuite(const std::string& name)
  {
    RNode* temp;
    current_node->AddNext(name, temp);
    current_node = temp;
  }

  void CloseSuite()
  {
    current_node = current_node->parent;
  }

  void AddCase(BenchmarkCase* bc)
  {
    RNode* c;
    bool res = current_node->AddNext(bc->Name(), c);

    if(res)
      total_nodes++;

    size_t l_id;
    auto it = l_map.find(bc->LibraryName());
    if(it != l_map.end())
      l_id = it->second;
    else
    {
      l_map[bc->LibraryName()] = current_l_id++;
      l_id = current_l_id - 1;
    }

    c->AddCase(bc, l_id);
  }

  void Run()
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

  void PrintTXT(std::ostream& stream)
  {
    stream << std::setiosflags(std::ios::left);
    stream << "######################################################################"
           << std::endl;
    stream << "#  " << std::setw(66) << "Manak C++ Benchmarking Library" << "#"
           << std::endl;

    std::stringstream ss;
    ss << "Version " << __MANAK_VERSION_MAJOR << "." << __MANAK_VERSION_MINOR
       << "." << __MANAK_VERSION_PATCH;
    stream << "#  " << std::setw(66) << ss.str() << "#" << std::endl;

    std::stringstream ss2;
    ss2 << "Created at " << Timer::getTimeStamp();

    stream << "#  " << std::setw(66) << ss2.str() << "#" << std::endl;

    stream << "######################################################################"
           << std::endl << std::endl;

    stream << std::setprecision(3);
    stream << std::setw(30) << "       Case Name";

    for(size_t i = 0;i < l_map.size();i++)
    {
      stream << std::setw(20);
      for(auto it : l_map)
      {
        if(it.second == i)
        {
          stream << it.first;
          break;
        }
      }
    }

    stream << std::endl;
    root->PrintTXT(stream, l_map.size());
  }

  void PrintHTML(std::ostream& stream)
  {
    stream << "<!DOCTYPE html>" << std::endl;

    //! Open html
    stream << "<html>" << std::endl;

    //! open head
    stream << "<head>" << std::endl;

    //! add title
    stream << "<title> Benchmarking log for module '"
           << MANAK_MODULE_NAME << "'"
           << "</title>" << std::endl;

    //! add styles
    stream << "<style>" << std::endl;
    stream << "li bold {font-weight: bold; font-size: 25px; }" << std::endl;
    stream << "li info {font-size: 25px; }" << std::endl;
    stream << "table, th, td { border: 1px solid black; \
                               border-collapse: collapse; \
                             } \
               th, td { \
                        padding: 5px; \
                        text-align: left; \
                      }" << std::endl;
    stream << "</style>" << std::endl;

    //! close head
    stream << "</head>" << std::endl;

    //! Add body
    stream << "<body>" << std::endl;

    //! Add heading
    stream << "<h1 style=\"text-align:center\"> Benchmarking Log for Module '"
           << MANAK_MODULE_NAME << "'" << "</h1>" << std::endl;

    //! add blank line
    stream << "<br>" << std::endl;

    //! Add list of important information to show
    stream << "<ul>" << std::endl;

    //! add module name
    stream << "<li><bold>Module: </bold><info>" << MANAK_MODULE_NAME << "</info>"
           << "</li>" << std::endl;
    //! add timestamp
    stream << "<li><bold>Timestamp: </bold><info>"
           << Timer::getTimeStamp() << "</info></li>" << std::endl;
    //! add benchmark case count
    stream << "<li><bold>Total Cases: </bold><info>"
           << total_nodes << "</info></li>" << std::endl;

    //! close list
    stream << "</ul>" << std::endl;

    stream << "<br>" << std::endl;

    //! add table
    stream << "<table style=\"width:100%\">" << std::endl;

    //! table headings
    stream << "<tr>" << std::endl;
    stream << "<th>Benchmark Name</th>" << std::endl;
    stream << "<th></th>" << std::endl;
    for(size_t i = 0;i < l_map.size();i++)
    {
      stream << std::setw(20);
      for(auto it : l_map)
      {
        if(it.second == i)
        {
          stream << "<th>" << it.first << "</th>" << std::endl;
          break;
        }
      }
    }
    stream << "</tr>" << std::endl;

    root->PrintHTML(stream, l_map.size());

    stream << "</table>" << std::endl;

    //! close body
    stream << "</body>" << std::endl;

    //! close html
    stream << "</html>" << std::endl;
  }

  void SaveForComparison(std::ostream& stream)
  {
    stream << GetVersionInfo() << std::endl;

    stream << Timer::getTimeStamp() << std::endl;

    root->SaveForComparison(stream, "");
  }

  void LoadForComparison(std::istream& stream)
  {
    std::string temp;

    getline(stream, temp);
    getline(stream, temp);
    getline(stream, temp);

    //! extract time
    getline(stream, temp);

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

 private:
  RNode* root;
  RNode* current_node;

  std::map<std::string, size_t> l_map;

  size_t total_nodes;
  size_t current_l_id;
};

}


#endif // _MANAK_RUN_TREE_HPP_INCLUDED
