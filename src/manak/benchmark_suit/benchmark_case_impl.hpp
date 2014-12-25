namespace manak
{

std::list<utils::ObjectStore> BenchmarkCase::Run()
{
  utils::ObjectStore& os = utils::ObjectStore::GetGlobalObjectStore();

  std::list<utils::ObjectStore> out;

  os.Insert("Timer_CurrentTolerance", &tolerance, "Current_Run");
  os.Insert("Timer_CurrentResultList", &out, "Current_Run");

  for(auto run_function : run_functions)
  {
    MANAK_OPEN_LOG;
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::streambuf *cerrbuf = std::cout.rdbuf();

    std::cout.rdbuf(MANAK_BENCHMARK_REDIRECTION_STREAM);
    std::cerr.rdbuf(MANAK_BENCHMARK_REDIRECTION_STREAM);

    os["Timer_CurrentSubName"] = &run_function.first;

    Timer::Initialize(iterations);
    auto& fun = run_function.second;
    do
    {
      Timer::StartIter();
      Timer::StartTimer();

      fun();

      Timer::StopTimer();
    }while(Timer::EndIter());

    std::cout.rdbuf(coutbuf);
    std::cerr.rdbuf(cerrbuf);

    MANAK_CLOSE_LOG;

    Timer::Deinitialize();
  }

  os.EraseGroup("Current_Run");

  return out;
}

}
