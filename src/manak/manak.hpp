/**
 * @file manak.hpp
 * @author Sumedh Ghaisas
 *
 * Standalone header for end user.
 */
#ifndef MANAK_HPP_INCLUDED
#define MANAK_HPP_INCLUDED

#include <functional>

#include <manak/benchmark_suit/benchmark_suite.hpp>

//! If MANAK_ALTERNATE_INIT_FUNCTION is defined then that function will be
//! called from generated main function for initializing unit benchmarking
//! module. This function can be used manually registering cases.
#ifdef MANAK_ALTERNATE_INIT_FUNCTION
#define MANAK_INIT_FUNCTION MANAK_ALTERNATE_INIT_FUNCTION
#else
#define MANAK_INIT_FUNCTION manak::init_benchmarking_module
#endif // MANAK_ALTERNATE_INIT_FUNCTION


namespace manak /** C++ Unit Benchmarking Library. **/
{

//! This function runs benchmarking module with given function. If main is set
//! to auto, this function will be called else it has to be called manually.
//! Returns status as int
//!
//! \return int
//!
//!
int manak_benchmarking_main(std::function<bool()> init_func, int argc, char* argv[] )
{
  init_func();
  BenchmarkSuite::GetMasterSuite()->Run();
  return 0;
}

}; // namespace manak

//! If main generation is set to auto following function will be added
#ifdef MANAK_AUTO_BENCHMARK_MAIN

int main(int argc, char* argv[])
{
  return manak::manak_benchmarking_main(&MANAK_INIT_FUNCTION, argc, argv);
}

#endif // MANAK_AUTO_BENCHMARK_MAIN

#endif // MANAK_HPP_INCLUDED
