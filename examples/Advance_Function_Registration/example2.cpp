bool man_init();

#define MANAK_AUTO_MAIN
#define MANAK_INIT
#define MANAK_SIMPLE_MODULE example1
#define MANAK_MANUAL_INIT_FUNCTION man_init

#include <manak/manak.hpp>

class A
{
 public:
  static void static_fun()
  {
    
  }
  
  void fun(int a)
  {
  
  }
};

//! Registration of static class member function
MANAK_ADD_CASE(MANAK_BENCHMARK_CASE(B1, A::static_fun));

bool man_init()
{
  A* a = new A();
  //! Registration class member function after binding it to a given object
  manak::BenchmarkSuite::GetMasterSuite()->AddCase(MANAK_CREATE_BENCHMARK_WITH_TEMPLATE(B2, manak::utils::BindToObject(&A::fun, a))->AddArgs(1000));
}
