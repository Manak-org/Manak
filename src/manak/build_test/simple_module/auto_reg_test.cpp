#include <iostream>

#define MANAK_SIMPLE_MODULE auto_reg_test
#define MANAK_AUTO_MAIN

#include <manak/manak.hpp>

MANAK_AUTO_BENCHMARK_CASE(InMasterSuite)
{
  
}

MANAK_AUTO_SUITE(Suite1);

MANAK_AUTO_BENCHMARK_CASE(InSuite1)
{

}

MANAK_AUTO_SUITE_END();
