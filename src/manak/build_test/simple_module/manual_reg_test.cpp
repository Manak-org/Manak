#include <iostream>

#define MANAK_SIMPLE_MODULE manual_reg_test
#define MANAK_MANUAL_INIT_FUNCTION module_init

#include "../manak_test.hpp"

bool module_init();

#include <manak/manak.hpp>

bool module_init()
{
  std::cout << "testing" << std::endl;
}
