/**
 * @file manak.hpp
 * @author Sumedh Ghaisas
 *
 * Standalone header for end user.
 */
#ifndef MANAK_HPP_INCLUDED
#define MANAK_HPP_INCLUDED

#include <functional>

#include <manak/util/manak_env.hpp>
#include <manak/util/set_env.hpp>

#include <manak/manak_suite/manak_suite.hpp>
#include <manak/manak_suite/result_collector.hpp>
#include <manak/manak_suite/manak_macros.hpp>

#include <manak/benchmark_suite/benchmark_suite.hpp>

#include <manak/test_suite/test_suite.hpp>

#include <manak/output_manager/output_manager.hpp>

#include <manak/util/cli.hpp>

namespace manak /** C++ Unit Benchmarking Library. **/
{

/**
 * Default initialization function. Manual initialization function is called
 * from this function.
 */
bool manak_init_module()
{
  #ifndef MANAK_SIMPLE_MODULE
  #ifndef MANAK_MODULE
  static_assert(false, "Manak module not defined. Define either MANAK_MODULE or MANAK_SIMPLE_MODULE to appropriate name.");
  #endif // MANAK_MODULE
  #endif // MANAK_BENCHMARK_MODULE

  //! for configuration file
  #ifdef MANAK_CONFIG_FILE
  #include MANAK_CONFIG_FILE
  #endif // MANAK_CONFIG_FILE

  //! If MANAK_MANUAL_INIT_FUNCTION is defined then that function will be
  //! called from default initialization function. This function can be used
  //! manually registering cases.
  #ifdef MANAK_MANUAL_INIT_FUNCTION
  MANAK_MANUAL_INIT_FUNCTION();
  #endif // MANAK_MANUAL_INIT_FUNCTION

  manak::ManakEnv::GlobalEnv().GetModuleName() = MANAK_MODULE_NAME;

  return true;
}

//! This function runs benchmarking module with given function. If main is set
//! to auto, this function will be called else it has to be called manually.
//! Returns status as int
//!
//! \return int
//!
int manak_main(int argc, char* argv[] )
{
  bool output_format_html = true;
  bool compare = false;

  if(manak::utils::cli::CLI::cmdOptionExists(argv, argv + argc, "-h"))
  {
    std::cout << "-h          : For help" << std::endl;
    std::cout << "-r regex    : Only the cases satisfying the given regex will run." << std::endl;
    std::cout << "              Check out guide for supported regex expressions." << std::endl;
    std::cout << "-o filename : Save output to given file." << std::endl;
    std::cout << "-s filename : Save the result for comparison." << std::endl;
    std::cout << "-c filename : Load the given file for comparison." << std::endl;
    std::cout << "-f HTML     : Save the output as HTML(Default)" << std::endl;
    std::cout << "   TXT      : Save the output as text document" << std::endl;
    exit(0);
  }

  if(manak::utils::cli::CLI::cmdOptionExists(argv, argv + argc, "-v"))
  {
    std::cout << manak::GetVersionInfo() << std::endl;
    exit(0);
  }

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

  if(manak::utils::cli::CLI::cmdOptionExists(argv, argv + argc, "-f"))
  {
    std::string format = manak::utils::cli::CLI::getCmdOption(argv, argv + argc, "-f");
    if(format == "TXT" || format == "txt")
      output_format_html = false;
    else if(format == "HTML" || format == "html");
    else
    {
      std::cerr << "Unrecognized format \"" << format << "\"" << std::endl;
      std::cerr << "For supported formats pass \"-h\"" << std::endl;
      exit(1);
    }
  }

  std::string filename;

  if(manak::utils::cli::CLI::cmdOptionExists(argv, argv + argc, "-o"))
  {
    filename = std::string(manak::utils::cli::CLI::getCmdOption(argv, argv + argc, "-o"));
  }
  else
  {
    filename = MANAK_STRINGIZE(MANAK_DEFAULT_OUT_FILENAME);

    if(output_format_html)
    {
      filename += ".html";
    }
    else
    {
      filename += ".txt";
    }
  }

  if(output_format_html)
  {
    manak::OutputManager::GlobalOutputManager().AddHandler(new manak::HTMLOutputHandler(filename));
    manak::OutputManager::GlobalOutputManager().AddHandler(new manak::TXTOutputHandler("test.txt"));
  }
  else
  {
    manak::OutputManager::GlobalOutputManager().AddHandler(new manak::TXTOutputHandler(filename));
  }

  // call the default init function
  manak::manak_init_module();

  // Construct the running tree
  ManakSuite::GetMasterSuite().Run("", pattern, compare);

  // Run the generated run tree and collect results
  ResultCollector::GlobalResultCollector().Run();

  if(manak::utils::cli::CLI::cmdOptionExists(argv, argv + argc, "-c"))
  {
    std::string filename(manak::utils::cli::CLI::getCmdOption(argv, argv + argc, "-c"));
    std::ifstream com_file(filename);
    ResultCollector::GlobalResultCollector().LoadForComparison(com_file);
  }

  // Pass the results to registered output handlers
  ResultCollector::GlobalResultCollector().Print();

  if(manak::utils::cli::CLI::cmdOptionExists(argv, argv + argc, "-s"))
  {
    std::string filename(manak::utils::cli::CLI::getCmdOption(argv, argv + argc, "-s"));

    std::ofstream ss(filename);
    ResultCollector::GlobalResultCollector().SaveForComparison(ss);
    ss.close();
  }

  return 0;
}

}; // namespace manak

//! If main generation is set to auto following function will be added
#ifdef MANAK_AUTO_MAIN

int main(int argc, char* argv[])
{
  return manak::manak_main(argc, argv);
}

#endif // MANAK_AUTO_MAIN

#endif // MANAK_HPP_INCLUDED
