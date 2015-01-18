namespace manak
{

void OutputManager::AddHandler(OutputHandler* handler)
{
  handlers.push_back(handler);
}

void OutputManager::Initialize(bool compare,
                               const std::string& c_time)
{
  for(auto oh : handlers)
  {
    oh->Initialize(compare, c_time);
  }
}

void OutputManager::AddCase(const std::map<std::string, ManakCase*>& children,
                            const std::map<std::string, std::list<utils::ObjectStore>>& results)
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
