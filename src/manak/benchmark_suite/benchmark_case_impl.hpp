#include <manak/test_suite/test_suite.hpp>

namespace manak
{

std::list<utils::ObjectStore> BenchmarkCase::Run()
{
  utils::ObjectStore& os = utils::ObjectStore::GetGlobalObjectStore();

  TestMonitor& tm = TestMonitor::GetGlobalTestMonitor();

  std::list<utils::ObjectStore> out;

  //os.Insert("Timer_CurrentTolerance", &tolerance, "Current_Run");
  //os.Insert("Timer_CurrentResultList", &out, "Current_Run");
  //os.Insert("Timer_CurrentIterations", &iterations, "Current_Run");
  //os.Insert("Timer_CurrentSP", &sp, "Current_Run");

  for(auto run_function : run_functions)
  {
    MANAK_OPEN_LOG;
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::streambuf *cerrbuf = std::cout.rdbuf();

    std::cout.rdbuf(MANAK_BENCHMARK_REDIRECTION_STREAM);
    std::cerr.rdbuf(MANAK_BENCHMARK_REDIRECTION_STREAM);

    //os["Timer_CurrentSubName"] = new std::string(run_function.first);

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
      os["test_res"] = new bool(false);
    }

    out.emplace_back(os);
  }

//  os.EraseGroup("Current_Run");
//
//  double* t_tol = (double*)os.Get("Timer_CurrentSubTolerance");
//  if(t_tol != NULL)
//    delete t_tol;
//
//  size_t* t_iter = (size_t*)os.Get("Timer_CurrentSubIterations");
//  if(t_iter != NULL)
//    delete t_iter;

  return out;
}

}
