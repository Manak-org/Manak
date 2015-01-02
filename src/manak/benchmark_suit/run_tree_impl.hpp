namespace manak
{

///////////////////////////////////////////////////////////////////////////////
/// RNode Implementations
///////////////////////////////////////////////////////////////////////////////

RNode::~RNode()
{
  for(auto it : nexts)
  {
    delete it.second;
  }

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
    }
  }
}

bool RNode::AddNext(const std::string& name, RNode*& n)
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

std::string RNode::GetPMRep(const utils::ObjectStore& entry)
{
  std::stringstream ss;

  double comp_val = *(double*)entry.Get("compare");
  PMeasure pm = *(PMeasure*)entry.Get("pmeasure");

  if(comp_val >= 0)
  {
    double tol = *(double*)entry.Get("tolerance");

    int res = pm.Compare(comp_val, tol);

    if(res > 0)
      ss << "+";
    else if(res < 0)
      ss << "-";
    ss << pm << "(" << comp_val << ")";
  }
  else
    ss << pm;
  return ss.str();
}

void RNode::Run()
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

void RNode::PrintTXT(std::ostream& stream, size_t l_ids)
{
  for(auto it : nexts)
  {
    it.second->PrintTXT(stream, l_ids);
  }

  if(children.size() != 0)
  {
    std::list<utils::ObjectStore> dummy;
    std::list<utils::ObjectStore>::iterator it_s[l_ids];

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
                if(sub_name != "" && sub_name != *(std::string*)it_s[i]->Get("name"))
                  name_mismatch = true;
                else
                  sub_name = *(std::string*)it_s[i]->Get("name");
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

void RNode::PrintHTML(std::ostream& stream,
                      std::ostream& stream2,
                      size_t l_ids,
                      const std::map<std::string, size_t>& l_map)
{
  for(auto it : nexts)
  {
    it.second->PrintHTML(stream, stream2, l_ids, l_map);
  }

  if(children.size() != 0)
  {
    std::list<utils::ObjectStore> dummy;
    std::list<utils::ObjectStore>::iterator it_s[l_ids];

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

    stream2 << "<h2>" << children.begin()->second->UName() << "</h2>" << std::endl;
    stream2 << "<bold>Number of Libraries: </bold>" << l_ids - 1
            << "<br>" << std::endl;
    stream2 << "<bold>Libraries: </bold>";

    bool temp = true;
    for(auto it : results)
    {
      if(!temp)
      {
        stream2 << ", ";
      }
      else temp = false;

      for(auto it2 : l_map)
      {
        if(it2.second == it.first)
        {
          stream2 << it2.first;
        }
      }
    }
    stream2 << "<br>" << std::endl;

    for(auto lid : results)
    {
      std::string l_name = "";

      for(auto it : l_map)
        if(it.second == lid.first)
          l_name = it.first;

      stream2 << "<h3>" << l_name << "</h3>" << std::endl;

      //! add table
      stream2 << "<table style=\"width:100%\">" << std::endl;

      stream2 << "<tr>" << std::endl;
      stream2 << "<th>Subcase</th>" << std::endl;
      stream2 << "<th>Result</th>" << std::endl;
      stream2 << "<th>Min</th>" << std::endl;
      stream2 << "<th>Max</th>" << std::endl;
      stream2 << "<th>Tolerance</th>" << std::endl;
      stream2 << "<th>Iterations</th>" << std::endl;
      stream2 << "</tr>" << std::endl;

      size_t p_index = 0;

      for(auto res : lid.second)
      {
        utils::ObjectStore& os = res;

        std::string name = *(std::string*)res["name"];
        PMeasure pm = *(PMeasure*)res["pmeasure"];
        double tol = *(double*)res["tolerance"];
        size_t iter = *(size_t*)res["iterations"];

        stream2 << "<tr>" << std::endl;
        if(name != "")
          stream2 << "<td>" << name << "</td>" << std::endl;
        else
          stream2 << "<td>Parameter Set " << p_index << "</td>" << std::endl;

        stream2 << "<td>" << GetPMRep(os) << "</td>" << std::endl;
        stream2 << "<td>" << pm.min << "</td>" << std::endl;
        stream2 << "<td>" << pm.max << "</td>" << std::endl;
        stream2 << "<td>" << tol << "</td>" << std::endl;
        stream2 << "<td>" << iter << "</td>" << std::endl;
        stream2 << "</tr>" << std::endl;

        p_index++;
      }

      stream2 << "</table>" << std::endl;
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
                if(sub_name != "" && sub_name != *(std::string*)it_s[i]->Get("name"))
                  name_mismatch = true;
                else
                  sub_name = *(std::string*)it_s[i]->Get("name");
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

    stream2 << "<hr width=100% align=left>" << std::endl;
  }
}

void RNode::SaveForComparison(std::ostream& stream, const std::string& uname)
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
        stream << " " << ((PMeasure*)res.Get("pmeasure"))->avg;
      }
      stream << std::endl;
    }
  }
}

void RNode::LoadForComparison(const std::string& uname,
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
          *(double*)l_it.Get("compare") = *r_it;
        }
        else break;
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
/// RunTree Implementations
////////////////////////////////////////////////////////////////////////////////

void RunTree::AddCase(BenchmarkCase* bc)
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

void RunTree::Run()
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

void RunTree::PrintTXT(std::ostream& stream)
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

void RunTree::PrintHTML(std::ostream& stream)
{
  std::stringstream stream1;
  std::stringstream stream2;
  root->PrintHTML(stream1, stream2, l_map.size(), l_map);

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

  //! List styles
  stream << "li bold {font-weight: bold; font-size: 25px; }" << std::endl;
  stream << "li info {font-size: 25px; }" << std::endl;

  //! Table styles
  stream << "table, th, td { border: 1px solid black; \
                               border-collapse: collapse; \
                             } \
               th, td { \
                        padding: 5px; \
                        text-align: left; \
                      }" << std::endl;

  stream << "bold {font-weight: bold; }" << std::endl;

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

  if(isComp)
  {
    stream << "<li><bold>Timestamp of module added for comparison: "
           << compare_time << "</bold></li>" << std::endl;
  }

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

  stream << stream1.str() << std::endl;

  stream << "</table>" << std::endl;

  stream << "<h2>Detailed Report</h2>" << std::endl;
  stream << "<hr width=100% align=left>" << std::endl;

  stream << stream2.str() << std::endl;

  stream << "<footer>" << std::endl;
  stream << "<p>Created by Manak:C++ Unit Benchmarking Library</p>" << std::endl;
  stream << "</footer>" << std::endl;


  //! close body
  stream << "</body>" << std::endl;

  //! close html
  stream << "</html>" << std::endl;
}

void RunTree::SaveForComparison(std::ostream& stream)
{
  stream << GetVersionInfo() << std::endl;

  stream << Timer::getTimeStamp() << std::endl;

  root->SaveForComparison(stream, "");
}

void RunTree::LoadForComparison(std::istream& stream)
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

}
