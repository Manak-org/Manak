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

  stream << "<h2>Comparison Report</h2>" << std::endl;

  stream << stream1.str() << std::endl;

  stream << "<br>" << std::endl;

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

  //! generate compact table of this case
  std::list<utils::ObjectStore>::const_iterator* it_s = new std::list<utils::ObjectStore>::const_iterator[results.size()];

  {
    size_t index = 0;
    for(auto res = results.begin();res != results.end();res++)
    {
      it_s[index++] = res->second.begin();
    }
  }

  //! add a benchmark name heading
  stream1 << "<a href=\"#detail_" << uname << "\">"
          << "<h2>" << name << "</h2></a><br>"
          << std::endl;

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

        bool is_test = false;
        bool test_res = false;
        double sp;

        if(it_s[i] != res_it->second.end())
        {
          sp = *(double*)it_s[i]->Get("sp");
          is_test = *(bool*)it_s[i]->Get("is_test");

          if(is_test)
            test_res = ((TestResult*)it_s[i]->Get("test_res"))->GetStatus(sp);

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

        s_values << "<td";
        if(is_test)
        {
          if(test_res)
            s_values << " style=\"background-color:#00FF00\">";
          else
          {
            s_values << " style=\"background-color:#FF3030\">";
          }
        }
        else s_values << ">";
        s_values << ss.str() << "</td>" << std::endl;

        res_it++;
      }

      std::stringstream s_sub_name;
      if(!name_mismatch && sub_name != "")
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
      const utils::ObjectStore& os = *(res.second.begin());
      bool is_test = *(bool*)os.Get("is_test");
      bool sp = *(double*)os.Get("sp");
      bool test_res;
      if(is_test)
        test_res = ((TestResult*)os.Get("test_res"))->GetStatus(sp);

      stream1 << "<td";
      if(is_test)
      {
        if(test_res)
          stream1 << " style=\"background-color:#00FF00\">";
        else
        {
          stream1 << " style=\"background-color:#FF3030\">";
        }
      }
      else stream1 << ">";
      stream1 << GetPMRep(os) << "</td>" << std::endl;
    }

    stream1 << "</tr>" << std::endl;
  }

  stream1 << "</table>" << std::endl;
  stream1 << "<hr width=100% align=left>" << std::endl;


  //! Generate detailed report section
  stream2 << "<a name=\"detail_" << uname << "\">" << std::endl;
  stream2 << "<h2>" << uname << "</h2>" << std::endl;
  stream2 << "<bold>Number of Libraries: </bold>" << results.size()
          << "<br>" << std::endl;
  stream2 << "<bold>Libraries: </bold>";

  {
    bool temp = true;
    for(auto it : results)
    {
      if(!temp)
      {
        stream2 << ", ";
      }
      else temp = false;

      stream2 << "<a href=\"#" << uname << "_lib_" << it.first << "\">"
              << it.first << "</a>" << std::endl;
    }
    stream2 << "<br>" << std::endl;
  }

  for(auto lib : results)
  {
    std::string l_name = lib.first;

    stream2 << "<a name=\"" << uname << "_lib_" << l_name << "\">"
            << "<h3>" << l_name << "</h3>" << std::endl;

    //! add table
    stream2 << "<table style=\"width:100%\">" << std::endl;

    stream2 << "<tr>                                                          \
                  <th>Subcase</th>                                            \
                  <th>Result</th>                                             \
                  <th>Min</th>                                                \
                  <th>Max</th>                                                \
                  <th>Tolerance</th>                                          \
                  <th>Iterations</th>                                         \
                </tr>" << std::endl;


    std::stringstream stream21, stream22;

    size_t p_index = 0;

    for(auto res : lib.second)
    {
      utils::ObjectStore& os = res;

      std::string sub_name = *(std::string*)res["name"];
      PMeasure pm = *(PMeasure*)res["pmeasure"];
      double tol = *(double*)res["tolerance"];
      size_t iter = *(size_t*)res["iterations"];
      size_t sp = *(double*)res["sp"];
      bool is_test = *(bool*)res["is_test"];
      TestResult& test_res = *(TestResult*)res["test_res"];

      bool test_res_b;
      if(is_test)
        test_res_b = test_res.GetStatus(sp);

      if(is_test && !test_res_b)
      {
        stream21 << "<tr style=\"background-color:#FF3030\">" << std::endl;
      }
      else if(is_test && test_res_b)
      {
        stream21 << "<tr style=\"background-color:#00FF00\">" << std::endl;
      }
      else stream21 << "<tr>" << std::endl;

      if(lib.second.size() == 1)
      {
        stream21 << "<td>" << name << "</td>" << std::endl;
      }
      else if(sub_name != "")
      {
        stream21 << "<td><a href = \"#" << uname << "_lib_" << l_name << "_SC_"
                 << sub_name << "\">"
                 << sub_name << "</td></a>" << std::endl;

        if(is_test)
        {
          stream22 << "<a name = \"" << uname << "_lib_" << l_name << "_SC_"
                   << sub_name << "\">"
                   << "<h4>" << sub_name << "</h4></a>" << std::endl;
        }
      }
      else
      {
        stream21 << "<td><a href = \"#" << uname << "_lib_" << l_name << "_SC_"
                 << "Parameter Set " << p_index << "\">"
                 << "Parameter Set " << p_index << "</td></a>" << std::endl;

        if(is_test)
        {
          stream22 << "<a name = \"" << uname << "_lib_" << l_name << "_SC_"
                   << "Parameter Set " << p_index << "\">"
                   << "<h4>Parameter Set " << p_index << "</h4></a>" << std::endl;
        }
      }

      stream21 << "<td>" << GetPMRep(os) << "</td>" << std::endl;
      stream21 << "<td>" << pm.min << "</td>" << std::endl;
      stream21 << "<td>" << pm.max << "</td>" << std::endl;
      stream21 << "<td>" << tol << "</td>" << std::endl;
      stream21 << "<td>" << iter << "</td>" << std::endl;
      stream21 << "</tr>" << std::endl;

      if(is_test)
      {
        stream22 << "<ul>                                                     \
                      <li>Status: ";
        if(test_res_b)
          stream22 << "PASS";
        else
          stream22 << "FAIL";

        stream22 << "</li>" << std::endl;

        stream22 << "<li>Expected success percentage: " << sp << "</li>" << std::endl;
        stream22 << "<li>Observed success percentage: " << test_res.GetSP() * 100 << "</li>" << std::endl;

        stream22 << "</ul>" << std::endl;

        std::list<std::string> msgs;
        size_t n_itr;
        if(test_res.GetFailMsg(n_itr, msgs))
        {
          stream22 << "Failure Msg: " << std::endl;
          stream22 << "<ul>" << std::endl;

          for(auto msg : msgs)
          {
            stream22 << "<li>" << msg << "</li>" << std::endl;
          }
          stream22 << "</ul>" << std::endl;
        }
      }

      p_index++;
    }

    stream2 << stream21.str();
    stream2 << "</table>" << std::endl;

    stream2 << stream22.str() << std::endl;

    stream2 << "</a>" << std::endl;
  }

  stream2 << "</a>" << std::endl;
  stream2 << "<hr width=100% align=left>" << std::endl;
}

std::string HTMLOutputHandler::GetPMRep(const utils::ObjectStore& entry)
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

}
