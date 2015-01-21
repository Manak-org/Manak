namespace manak
{

void TXTOutputHandler::Initialize(bool compare,
                                  const std::string& c_time)
{
  //! Print information about manak version which generated this output
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

//  stream << std::setprecision(3);
//  stream << std::setw(30) << "       Case Name";
//
//  for(size_t i = 0;i < l_map.size();i++)
//  {
//    stream << std::setw(20);
//    for(auto it : l_map)
//    {
//      if(it.second == i)
//      {
//        stream << it.first;
//        break;
//      }
//    }
//  }

  stream << std::endl;
}

void TXTOutputHandler::AddCase(const std::string& uname,
                               const std::string& name,
                               const std::map<std::string, std::list<utils::ObjectStore>>& results)
{
  //! print case name and libraries involved
  stream << std::setprecision(3);
  stream << std::setw(30) << "       Case Name";

  for(auto child : results)
  {
    stream << std::setw(20);
    stream << child.first;
  }
  stream << std::endl;

  //! Check the highest number of sub-case count in all the libraries
  size_t num_entries = 0;
  for(auto l : results)
  {
    if(l.second.size() > num_entries)
      num_entries = l.second.size();
  }

  //! if entries are more than 1 we need to print their name of temp name
  //! if they don't have one
  if(num_entries > 1)
  {
    //! put iterators to the start of result lists
    std::list<utils::ObjectStore> dummy;
    std::list<utils::ObjectStore>::const_iterator it_s[results.size()];

    {
      size_t index = 0;
      for(auto res = results.begin();res != results.end();res++)
      {
        it_s[index++] = res->second.begin();
      }
    }

    //! print the case name on one line
    stream << name << std::endl;

    //! loop for max entries and put '---' if the entry is missing
    for(size_t entry_index = 0;entry_index < num_entries;entry_index++)
    {
      std::stringstream s_values;
      s_values << std::setiosflags(std::ios::left);

      //! the i'th sub-case name for all the libraries must be same
      //! or else temp name will be printed
      std::string sub_name = "";
      bool name_mismatch = false;

      //! check the entry_index entry of the result list and print accordingly
      auto it_res = results.begin();
      for(size_t i = 0;i < results.size();i++)
      {
        //! temp stringstream
        std::stringstream ss;

        //! check if there is an ith sub-case
        if(it_s[i] != it_res->second.end())
        {
          //! check for sub-case name mismatch
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

        s_values << std::setw(20) << ss.str();
        it_res++;
      }

      //! add the sub-case name
      //! in case mismatch provide 'parameter set i' where i is replaced by
      //! sub-case's chronological number
      std::stringstream s_sub_name;
      s_sub_name << "  ";
      if(!name_mismatch)
      {
        s_sub_name << sub_name;
      }
      else
      {
        s_sub_name << "Parameter Set " << entry_index;
      }

      stream << std::setw(30) << s_sub_name.str();
      stream << s_values.str();

      stream << std::endl;
    }
  }
  else
  {
    stream << std::setw(30) << name;

    for(auto res : results)
    {
      stream << std::setw(20) << GetPMRep(*(res.second.begin()));
    }

    stream << std::endl;
  }
  stream << std::endl;
}

std::string TXTOutputHandler::GetPMRep(const utils::ObjectStore& entry)
{
  std::stringstream ss;

  double comp_val = *(double*)entry.Get("compare");
  PMeasure pm = *(PMeasure*)entry.Get("pmeasure");
  bool is_test = *(bool*)entry.Get("is_test");
  bool test_res = true;

  if(is_test)
  {
    TestResult
    test_res = *(TestResult*)entry.Get("test_res");
  }

  if(test_res)
  {
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
  }
  else ss << "X";

  return ss.str();
}

}
