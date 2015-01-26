namespace manak
{

inline std::list<utils::ObjectStore> BenchmarkCase::Run()
{
  TestMonitor& tm = TestMonitor::GetGlobalTestMonitor();

  std::list<utils::ObjectStore> out;

  for(auto run_function : run_functions)
  {
    MANAK_OPEN_LOG;
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::streambuf *cerrbuf = std::cout.rdbuf();

    std::cout.rdbuf(MANAK_REDIRECTION_BUFFER);
    std::cerr.rdbuf(MANAK_REDIRECTION_BUFFER);

    Timer::Initialize();
    tm.Initialize();

    size_t c_iterations = iterations;

    auto& fun = run_function.second;
    do
    {
      Timer::StartIter();

      TestMonitor::GetGlobalTestMonitor().NewEntry();

      fun();

      TestMonitor::GetGlobalTestMonitor().ConfirmEntry();


    }while(Timer::EndIter(c_iterations));

    PMeasure pm = Timer::Deinitialize(iterations);

    std::cout.rdbuf(coutbuf);
    std::cerr.rdbuf(cerrbuf);

    MANAK_CLOSE_LOG;

    utils::ObjectStore os;
    os["name"] = new std::string(run_function.first);
    os["tolerance"] = new double(tolerance);
    os["pmeasure"] = new PMeasure(pm);
    os["compare"] = new double(-1);
    os["iterations"] = new size_t(iterations);
    os["sp"] = new double(success_p);

    if(TestMonitor::GetGlobalTestMonitor().IsTest())
    {
      TestResult tr = TestMonitor::GetGlobalTestMonitor().Result();
      os["is_test"] = new bool(true);
      os["test_res"] = new TestResult(tr);
    }
    else
    {
      os["is_test"] = new bool(false);
    }

    out.emplace_back(os);
  }

  return out;
}

}
