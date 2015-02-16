#define MANAK_AUTO_MAIN
#define MANAK_INIT
#define MANAK_SIMPLE_MODULE example1

#include <manak/manak.hpp>

template<typename T>
T fun(T a)
{
  std::cout << "fun1" << std::endl;
}

int fun(size_t a)
{
  std::cout << "fun2" << std::endl;
}

int fun(double a)
{
  std::cout << "fun3" << std::endl;
}

//! Will register function which prints "fun1"
MANAK_ADD_CASE(MANAK_CREATE_BENCHMARK_WITH_TEMPLATE(B2, fun<size_t>)->AddArgs(0));

//! Will register function which prints "fun2"
MANAK_ADD_CASE(MANAK_CREATE_BENCHMARK_WITH_TEMPLATE(B1, static_cast<int(&)(size_t)>(fun))->AddArgs(0));
