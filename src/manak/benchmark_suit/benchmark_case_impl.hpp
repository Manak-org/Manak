namespace manak
{

std::list<utils::ObjectStore> BenchmarkCase::Run()
{
  utils::ObjectStore& os = utils::ObjectStore::GetGlobalObjectStore();

  std::list<utils::ObjectStore> out;

  os.Insert("Timer_CurrentTolerance", &tolerance, "Current_Run");
  os.Insert("Timer_CurrentResultList", &out, "Current_Run");
  os.Insert("Timer_CurrentIterations", &iterations, "Current_Run");

  for(auto run_function : run_functions)
  {
    MANAK_OPEN_LOG;
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::streambuf *cerrbuf = std::cout.rdbuf();

    std::cout.rdbuf(MANAK_BENCHMARK_REDIRECTION_STREAM);
    std::cerr.rdbuf(MANAK_BENCHMARK_REDIRECTION_STREAM);

    os["Timer_CurrentSubName"] = new std::string(run_function.first);

    Timer::Initialize();
    auto& fun = run_function.second;
    do
    {
      Timer::StartIter();

      fun();
    }while(Timer::EndIter());

    std::cout.rdbuf(coutbuf);
    std::cerr.rdbuf(cerrbuf);

    MANAK_CLOSE_LOG;

    Timer::Deinitialize();
  }

  os.EraseGroup("Current_Run");

  double* t_tol = (double*)os.Get("Timer_CurrentSubTolerance");
  if(t_tol != NULL)
    delete t_tol;

  size_t* t_iter = (size_t*)os.Get("Timer_CurrentSubIterations");
  if(t_iter != NULL)
    delete t_iter;

  return out;
}

}
