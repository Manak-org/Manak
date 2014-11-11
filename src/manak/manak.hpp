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
#include <manak/util/log.hpp>

//! If MANAK_ALTERNATE_INIT_FUNCTION is defined then that function will be
//! called from generated main function for initializing unit benchmarking
//! module. This function can be used manually registering cases.
#ifdef MANAK_ALTERNATE_INIT_FUNCTION
#define MANAK_INIT_FUNCTION MANAK_ALTERNATE_INIT_FUNCTION
#else
#define MANAK_INIT_FUNCTION manak::init_benchmarking_module
#endif // MANAK_ALTERNATE_INIT_FUNCTION

#ifndef MANAK_DEFAULT_OUT_FILENAME
#define MANAK_DEFAULT_OUT_FILENAME benchmark_stat.txt
#endif

#include <manak/util/cli.hpp>

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
  bool compare = false;

  init_func();
  std::string pattern = "";
  if(manak::utils::cli::CLI::cmdOptionExists(argv, argv + argc, "-r"))
  {
    char* c_pattern = manak::utils::cli::CLI::getCmdOption(argv,
                                                           argv + argc,
                                                           "-r");
    if(c_pattern)
    {
      pattern = std::string(c_pattern);
    }
    else
    {
      std::cout << "No pattern given!" << std::endl;
      exit(1);
    }
  }

  std::ofstream* stream = NULL;

  if(manak::utils::cli::CLI::cmdOptionExists(argv, argv + argc, "-o"))
  {
    std::string filename(manak::utils::cli::CLI::getCmdOption(argv, argv + argc, "-o"));
    stream = new std::ofstream(filename);
  }
  else
  {
    stream = new std::ofstream(MANAK_STRINGIZE(MANAK_DEFAULT_OUT_FILENAME));
  }

  if(manak::utils::cli::CLI::cmdOptionExists(argv, argv + argc, "-c"))
  {
    compare = true;
    std::string filename(manak::utils::cli::CLI::getCmdOption(argv, argv + argc, "-c"));
    BenchmarkSuite::GetMasterSuite()->LoadData(filename);
  }

  BenchmarkSuite::GetMasterSuite()->Run("", pattern, compare);

  utils::Log::GetLog().Print(*stream);

  if(manak::utils::cli::CLI::cmdOptionExists(argv, argv + argc, "-s"))
  {
    std::string filename(manak::utils::cli::CLI::getCmdOption(argv, argv + argc, "-s"));

    std::ofstream ss(filename);
    utils::Log::GetLog().Save(ss);
    ss.close();
  }

  stream->close();
  delete stream;
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
