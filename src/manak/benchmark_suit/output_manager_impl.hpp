namespace manak
{

void OutputManager::AddHandler(OutputHandler* handler)
{
  handlers.push_back(handler);
}

void OutputManager::Initialize(const std::map<std::string, size_t>& l_map,
                               bool compare,
                               const std::string& c_time)
{
  for(auto oh : handlers)
  {
    oh->Initialize(l_map, compare, c_time);
  }
}

void OutputManager::AddCase(const std::map<size_t, BenchmarkCase*>& children,
                            const std::map<size_t, std::list<utils::ObjectStore>>& results)
{
  for(auto oh : handlers)
  {
    oh->AddCase(children, results);
  }
}

void OutputManager::Finalize()
{
  for(auto oh : handlers)
  {
    oh->Finalize();
  }
}

}
