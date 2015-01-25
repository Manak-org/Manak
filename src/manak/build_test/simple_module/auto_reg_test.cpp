#include <iostream>

#define MANAK_SIMPLE_MODULE auto_reg_test

#include "../manak_test.hpp"

#include <manak/manak.hpp>

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
