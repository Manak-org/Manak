#include <iostream>

#define MANAK_SIMPLE_MODULE auto_reg_test
#define MANAK_AUTO_MAIN

#define MANAK_DEFAULT_OUT_FILENAME sm_auto_reg_test

#include <manak/manak.hpp>

using namespace std;

MANAK_AUTO_BENCHMARK_CASE(InMasterSuite)
{
  
}

MANAK_AUTO_SUITE(Suite1);

MANAK_AUTO_BENCHMARK_CASE(InSuite1)
{

}

MANAK_AUTO_SUITE(Suite2);

MANAK_AUTO_BENCHMARK_CASE(InSuite2)
{

}

MANAK_AUTO_SUITE_END();

MANAK_AUTO_SUITE_END();

MANAK_AUTO_BENCHMARK_CASE_TIS(AutoBCaseTIS, 1, 1, 1)
{

}

MANAK_AUTO_BENCHMARK_CASE_TI(AutoBCaseTI, 1, 1)
{

}

MANAK_AUTO_BENCHMARK_CASE_IS(AutoBCaseIS, 1, 1)
{

}

MANAK_AUTO_BENCHMARK_CASE_T(AutoBCaseT, 1)
{

}

MANAK_AUTO_BENCHMARK_CASE_I(AutoBcaseI, 1)
{
  
}

void test(int a)
{

}

list<tuple<int>> GetCustomArgs(int a)
{
  list<tuple<int>> out;
  for(size_t i = 0;i < a;i++)
    out.emplace_back(i);
  return out;
}

MANAK_ADD_CASE(MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_TIS(AutoPBCaseTIS, test, 1, 1, 1)->AddArgs(0));

MANAK_ADD_CASE(MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_TI(AutoPBCaseTI, test, 1, 1)->AddCustomArgs(GetCustomArgs, 10));

MANAK_ADD_CASE(MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_IS(AutoPBCaseIS, test, 1, 1)->AddArgs(0));

MANAK_ADD_CASE(MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_T(AutoPBCaseT, test, 1)->AddArgs(0));

MANAK_ADD_CASE(MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_I(AutoPBCaseI, test, 1)->AddArgs(0));

MANAK_ADD_CASE(MANAK_CREATE_BENCHMARK_WITH_TEMPLATE(AutoPBCase, test)->AddArgs(0));

MANAK_AUTO_GROUP();

size_t count;

GINIT()
{
  count = 10;
}

MANAK_AUTO_GROUP_BENCHMARK_CASE_TIS("AutoGBCaseTIS", 1, 1, 1)
{

}

MANAK_AUTO_GROUP_BENCHMARK_CASE_TI("AutoGBCaseTI", 1, 1)
{

}

MANAK_AUTO_GROUP_BENCHMARK_CASE_IS("AutoGBCaseIS", 1, 1)
{

}

MANAK_AUTO_GROUP_BENCHMARK_CASE_T("AutoGBCaseT", 1)
{

}

MANAK_AUTO_GROUP_BENCHMARK_CASE_I("AutoGBCaseI", 1)
{

}

MANAK_AUTO_GROUP_BENCHMARK_CASE("AutoGBCase")
{

}

MANAK_AUTO_GROUP_END();
