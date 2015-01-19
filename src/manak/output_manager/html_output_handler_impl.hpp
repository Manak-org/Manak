namespace manak
{

void HTMLOutputHandler::Initialize(bool compare,
                                   const std::string& c_time)
{
  isComp = compare;
  compare_time = c_time;
}

void HTMLOutputHandler::Finalize()
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

  stream << stream1.str() << std::endl;

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

void HTMLOutputHandler::AddCase(const std::string& uname,
                                const std::string& name,
                                const std::map<std::string, std::list<utils::ObjectStore>>& results)
{

  total_nodes++;

  std::list<utils::ObjectStore> dummy;
  std::list<utils::ObjectStore>::const_iterator it_s[results.size()];

  {
    size_t index = 0;
    for(auto res : results)
    {
      it_s[index++] = res.second.begin();
    }
  }

  //! add a benchmark name heading
  stream1 << "<h2>" << name << "</h2><br>" << std::endl;

  //! construct case table
  stream1 << "                                                                \
  <table style=\"width:100%\">                                                \
  <tr>                                                                        \
    <th></th>" << std::endl;
  for(auto res : results)
  {
    stream1 << "<th>" << res.first << "</th>" << std::endl;
  }
  stream1 << "</tr>" << std::endl;

  //! Check the highest number of sub-case count in all the libraries
  size_t num_entries = 0;
  for(auto l : results)
  {
    if(l.second.size() > num_entries)
      num_entries = l.second.size();
  }

  //stream2 << "<h2>" << children.begin()->second->UName() << "</h2>" << std::endl;
  //stream2 << "<bold>Number of Libraries: </bold>" << l_ids - 1
            //<< "<br>" << std::endl;
  //stream2 << "<bold>Libraries: </bold>";

//  bool temp = true;
//  for(auto it : results)
//  {
//    if(!temp)
//    {
//      stream2 << ", ";
//    }
//    else temp = false;
//
//    for(auto it2 : l_map)
//    {
//      if(it2.second == it.first)
//      {
//        stream2 << it2.first;
//      }
//    }
//  }
//  stream2 << "<br>" << std::endl;

//  for(auto lib : results)
//  {
//    std::string l_name = lib.first;

    //stream2 << "<h3>" << l_name << "</h3>" << std::endl;

//    //! add table
//    stream2 << "<table style=\"width:100%\">" << std::endl;
//
//    stream2 << "<tr>" << std::endl;
//    stream2 << "<th>Subcase</th>" << std::endl;
//    stream2 << "<th>Result</th>" << std::endl;
//    stream2 << "<th>Min</th>" << std::endl;
//    stream2 << "<th>Max</th>" << std::endl;
//    stream2 << "<th>Tolerance</th>" << std::endl;
//    stream2 << "<th>Iterations</th>" << std::endl;
//    stream2 << "</tr>" << std::endl;

//    size_t p_index = 0;
//
//    for(auto res : lib.second)
//    {
//      utils::ObjectStore& os = res;
//
//      std::string name = *(std::string*)res["name"];
//      PMeasure pm = *(PMeasure*)res["pmeasure"];
//      double tol = *(double*)res["tolerance"];
//      size_t iter = *(size_t*)res["iterations"];
//
//      stream2 << "<tr>" << std::endl;
//      if(name != "")
//        stream2 << "<td>" << name << "</td>" << std::endl;
//      else
//        stream2 << "<td>Parameter Set " << p_index << "</td>" << std::endl;
//
//      stream2 << "<td>" << GetPMRep(os) << "</td>" << std::endl;
//      stream2 << "<td>" << pm.min << "</td>" << std::endl;
//      stream2 << "<td>" << pm.max << "</td>" << std::endl;
//      stream2 << "<td>" << tol << "</td>" << std::endl;
//      stream2 << "<td>" << iter << "</td>" << std::endl;
//      stream2 << "</tr>" << std::endl;
//
//      p_index++;
//    }
//
//    stream2 << "</table>" << std::endl;
//  }

  if(num_entries > 1)
  {
    stream1 << "<tr>" << std::endl;

    for(size_t index = 0;index < num_entries;index++)
    {
      std::stringstream s_values;

      std::string sub_name = "";
      bool name_mismatch = false;

      auto res_it = results.begin();
      for(size_t i = 0;i < results.size();i++)
      {
        std::stringstream ss;

        if(it_s[i] != res_it->second.end())
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

        s_values << "<td>" << ss.str() << "</td>" << std::endl;
        res_it++;
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

      stream1 << "<td>" << s_sub_name.str() << "</td>" << std::endl;
      stream1 << s_values.str() << std::endl;
      stream1 << "</tr>" << std::endl;
      if(index != num_entries - 1)
        stream1 << "<tr>" << std::endl;
    }
  }
  else
  {
    stream1 << "<tr>" << std::endl;
    stream1 << "<td>" << name << "</td>" << std::endl;

    for(auto res : results)
    {
      stream1 << "<td>" << GetPMRep(*(res.second.begin())) << "</td>" << std::endl;
    }

    stream1 << "</tr>" << std::endl;
  }

  stream1 << "</table>" << std::endl;
  stream1 << "<hr width=100% align=left>" << std::endl;

  //stream2 << "<hr width=100% align=left>" << std::endl;
}

}
