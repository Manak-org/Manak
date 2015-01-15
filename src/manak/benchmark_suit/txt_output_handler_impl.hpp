namespace manak
{

void TXTOutputHandler::Initialize(const std::map<std::string, size_t>& l_map,
                                  bool compare,
                                  const std::string& c_time)
{
  this->l_map = l_map;

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
}

void TXTOutputHandler::AddCase(const std::map<size_t, BenchmarkCase*>& children,
                               const std::map<size_t, std::list<utils::ObjectStore>>& results)
{
  size_t l_ids = l_map.size();

  if(children.size() != 0)
  {
    std::list<utils::ObjectStore> dummy;
    std::list<utils::ObjectStore>::const_iterator it_s[l_ids];

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

}
