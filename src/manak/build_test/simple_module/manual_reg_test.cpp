#include <iostream>

#define MANAK_SIMPLE_MODULE manual_reg_test
#define MANAK_MANUAL_INIT_FUNCTION module_init
#define MANAK_AUTO_MAIN

#define MANAK_DEFAULT_OUT_FILENAME sm_man_reg_test

bool module_init();

#include <manak/manak.hpp>

using namespace manak;

void fun()
{

}

bool module_init()
{
  ManakSuite& g = ManakSuite::GetMasterSuite();
  g.AddCase(MANAK_BENCHMARK_CASE(InMasterSuite, fun));
  
  ManakSuite* suite1 = g.AddSuite(MANAK_SUITE(Suite1));
  suite1->AddCase(MANAK_BENCHMARK_CASE(InSuite1, fun));
  
  ManakSuite* suite2 = g.AddSuite(MANAK_SUITE(Suite2));
  suite2->AddCase(MANAK_BENCHMARK_CASE(InSuite2, fun));
  
  return true;
}
