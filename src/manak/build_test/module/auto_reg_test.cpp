#include <iostream>

#define MANAK_MODULE auto_reg_test
#define MANAK_AUTO_MAIN
#define MANAK_INIT

#define MANAK_DEFAULT_OUT_FILENAME sm_auto_reg_test

#include <manak/manak.hpp>

using namespace std;

MANAK_AUTO_BENCHMARK_CASE(InMasterSuite, Lib1)
{
  
}

MANAK_AUTO_SUITE(Suite1);

MANAK_AUTO_BENCHMARK_CASE(InSuite1, Lib1)
{

}

MANAK_AUTO_BENCHMARK_CASE(InSuite1, Lib2)
{

}

MANAK_AUTO_SUITE(Suite2);

MANAK_AUTO_BENCHMARK_CASE(InSuite2, Lib1)
{

}

MANAK_AUTO_SUITE_END();

MANAK_AUTO_SUITE_END();

MANAK_AUTO_BENCHMARK_CASE_TIS(AutoBCaseTIS, Lib1, 1, 1, 1)
{

}

MANAK_AUTO_BENCHMARK_CASE_TI(AutoBCaseTI, Lib1, 1, 1)
{

}

MANAK_AUTO_BENCHMARK_CASE_IS(AutoBCaseIS, Lib1, 1, 1)
{

}

MANAK_AUTO_BENCHMARK_CASE_T(AutoBCaseT, Lib1, 1)
{

}

MANAK_AUTO_BENCHMARK_CASE_I(AutoBCaseI, Lib1, 1)
{
  
}

void test(int a)
{
  (void)a;
}

list<tuple<int>> GetCustomArgs(size_t a)
{
  list<tuple<int>> out;
  for(size_t i = 0;i < a;i++)
    out.emplace_back(i);
  return out;
}

MANAK_ADD_CASE(MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_TIS(AutoPBCaseTIS, Lib1, test, 1, 1, 1)->AddArgs(0));

MANAK_ADD_CASE(MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_TI(AutoPBCaseTI, Lib1, test, 1, 1)->AddCustomArgs(GetCustomArgs, 10));

MANAK_ADD_CASE(MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_IS(AutoPBCaseIS, Lib1, test, 1, 1)->AddArgs(0));

MANAK_ADD_CASE(MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_T(AutoPBCaseT, Lib1, test, 1)->AddArgs(0));

MANAK_ADD_CASE(MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_I(AutoPBCaseI, Lib1, test, 1)->AddArgs(0));

MANAK_ADD_CASE(MANAK_CREATE_BENCHMARK_WITH_TEMPLATE(AutoPBCase, Lib1, test)->AddArgs(0));

MANAK_AUTO_GROUP();

size_t count;

GINIT()
{
  count = 10;
}

void fun(size_t a)
{
  (void)a;
}

list<tuple<size_t>> GetCustomArgs()
{
  list<tuple<size_t>> out;
  for(size_t i = 0;i < 10;i++)
    out.emplace_back(i);
  return out;
}

MANAK_AUTO_GROUP_BENCHMARK_CASE_TIS("AutoGBCaseTIS", "Lib1", 1, 1, 1)
{

}

MANAK_AUTO_GROUP_BENCHMARK_CASE_TI("AutoGBCaseTI", "Lib1", 1, 1)
{

}

MANAK_AUTO_GROUP_BENCHMARK_CASE_IS("AutoGBCaseIS", "Lib1", 1, 1)
{

}

MANAK_AUTO_GROUP_BENCHMARK_CASE_T("AutoGBCaseT", "Lib1", 1)
{

}

MANAK_AUTO_GROUP_BENCHMARK_CASE_I("AutoGBCaseI", "Lib1", 1)
{

}

MANAK_AUTO_GROUP_BENCHMARK_CASE("AutoGBCase", "Lib1")
{

}

MANAK_ADD_TO_GROUP(MANAK_CREATE_GROUP_BENCHMARK_WITH_TEMPLATE_TIS("AutoGPBCaseTIS", "Lib1", fun, 1, 1, 1)->AddArgs(0));

MANAK_ADD_TO_GROUP(MANAK_CREATE_GROUP_BENCHMARK_WITH_TEMPLATE_TI("AutoGPBCaseTI", "Lib1", fun, 1, 1)->AddCustomArgs(MANAK_GROUP_FUNCTION(GetCustomArgs)));

MANAK_ADD_TO_GROUP(MANAK_CREATE_GROUP_BENCHMARK_WITH_TEMPLATE_IS("AutoGPBCaseIS", "Lib1", fun, 1, 1)->AddArgs(0));

MANAK_ADD_TO_GROUP(MANAK_CREATE_GROUP_BENCHMARK_WITH_TEMPLATE_T("AutoGPBCaseT", "Lib1", fun, 1)->AddArgs(0));

MANAK_ADD_TO_GROUP(MANAK_CREATE_GROUP_BENCHMARK_WITH_TEMPLATE_I("AutoGPBCaseI", "Lib1", fun, 1)->AddArgs(0));

MANAK_ADD_TO_GROUP(MANAK_CREATE_GROUP_BENCHMARK_WITH_TEMPLATE("AutoGPBCase", "Lib1", fun)->AddArgs(0));

MANAK_AUTO_GROUP_END();
