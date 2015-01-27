#include <iostream>

#define MANAK_MODULE manual_reg_test
#define MANAK_MANUAL_INIT_FUNCTION module_init
#define MANAK_AUTO_MAIN
#define MANAK_INIT

#define MANAK_DEFAULT_OUT_FILENAME sm_man_reg_test

bool module_init();

#include <manak/manak.hpp>

using namespace std;
using namespace manak;

void fun()
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

template<typename T>
MANAK_GROUP(TestGroup);

size_t count;
string name;

GINIT(const std::string& n)
{
  count = 10;
  name = n;
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

void fun2()
{

}

MANAK_ADD_TO_GROUP(MANAK_GROUP_BENCHMARK_CASE_TIS(name + "_GBCaseTIS", "Lib1", fun2, 1, 1, 1));
MANAK_ADD_TO_GROUP(MANAK_GROUP_BENCHMARK_CASE_TI(name + "_GBCaseTI", "Lib1", fun2, 1, 1));
MANAK_ADD_TO_GROUP(MANAK_GROUP_BENCHMARK_CASE_IS(name + "_GBCaseIS", "Lib1", fun2, 1, 1));
MANAK_ADD_TO_GROUP(MANAK_GROUP_BENCHMARK_CASE_T(name + "_GBCaseT", "Lib1", fun2, 1));
MANAK_ADD_TO_GROUP(MANAK_GROUP_BENCHMARK_CASE_I(name + "_GBCaseI", "Lib1", fun2, 1));
MANAK_ADD_TO_GROUP(MANAK_GROUP_BENCHMARK_CASE(name + "_GBCase", "Lib1", fun2));

MANAK_ADD_TO_GROUP(MANAK_CREATE_GROUP_BENCHMARK_WITH_TEMPLATE_TIS(name + "_GPBCaseTIS", "Lib1", fun, 1, 1, 1)->AddArgs(0));

MANAK_ADD_TO_GROUP(MANAK_CREATE_GROUP_BENCHMARK_WITH_TEMPLATE_TI(name + "_GPBCaseTI", "Lib1", fun, 1, 1)->AddCustomArgs(MANAK_GROUP_FUNCTION(GetCustomArgs)));

MANAK_ADD_TO_GROUP(MANAK_CREATE_GROUP_BENCHMARK_WITH_TEMPLATE_IS(name + "_GPBCaseIS", "Lib1", fun, 1, 1)->AddArgs(0));

MANAK_ADD_TO_GROUP(MANAK_CREATE_GROUP_BENCHMARK_WITH_TEMPLATE_T(name + "_GPBCaseT", "Lib1", fun, 1)->AddArgs(0));

MANAK_ADD_TO_GROUP(MANAK_CREATE_GROUP_BENCHMARK_WITH_TEMPLATE_I(name + "_GPBCaseI", "Lib1", fun, 1)->AddArgs(0));

MANAK_ADD_TO_GROUP(MANAK_CREATE_GROUP_BENCHMARK_WITH_TEMPLATE(name + "_GPBCase", "Lib1", fun)->AddArgs(0));

MANAK_GROUP_END();

MANAK_ADD_GROUP(TestGroup<size_t>, "size_t");
MANAK_ADD_GROUP(TestGroup<int>, "int");

bool module_init()
{
  ManakSuite& g = ManakSuite::GetMasterSuite();
  g.AddCase(MANAK_BENCHMARK_CASE(InMasterSuite, "Lib1", fun));

  ManakSuite* suite1 = g.AddSuite(MANAK_SUITE(Suite1));
  suite1->AddCase(MANAK_BENCHMARK_CASE(InSuite1, "Lib1", fun));

  ManakSuite* suite2 = g.AddSuite(MANAK_SUITE(Suite2));
  suite2->AddCase(MANAK_BENCHMARK_CASE(InSuite2, "Lib1", fun));

  g.AddCase(MANAK_BENCHMARK_CASE_TIS(BCaseTIS, "Lib1", fun, 1, 1, 1));
  g.AddCase(MANAK_BENCHMARK_CASE_TI(BCaseTI, "Lib1", fun, 1, 1));
  g.AddCase(MANAK_BENCHMARK_CASE_IS(BCaseIS, "Lib1", fun, 1, 1));
  g.AddCase(MANAK_BENCHMARK_CASE_T(BCaseT, "Lib1", fun, 1));
  g.AddCase(MANAK_BENCHMARK_CASE_I(BCaseI, "Lib1", fun, 1));

  g.AddCase(MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_TIS(PBCaseTIS, "Lib1", test, 1, 1, 1)->AddArgs(0));
  g.AddCase(MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_TI(PBCaseTI, "Lib1", test, 1, 1)->AddCustomArgs(GetCustomArgs, 10));
  g.AddCase(MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_IS(PBCaseIS, "Lib1", test, 1, 1)->AddArgs(0));
  g.AddCase(MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_T(PBCaseT, "Lib1", test, 1)->AddArgs(0));
  g.AddCase(MANAK_CREATE_BENCHMARK_WITH_TEMPLATE_I(PBCaseI, "Lib1", test, 1)->AddArgs(0));
  g.AddCase(MANAK_CREATE_BENCHMARK_WITH_TEMPLATE(PBCase, "Lib1", test)->AddArgs(0));

  return true;
}

