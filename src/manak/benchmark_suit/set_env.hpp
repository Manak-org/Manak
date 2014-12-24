#ifndef MANAK_SET_ENV_HPP_INCLUDED
#define MANAK_SET_ENV_HPP_INCLUDED

//! Set the module name for further use
#ifdef MANAK_SIMPLE_BENCHMARK_MODULE
#define MANAK_MODULE_NAME MANAK_STRINGIZE(MANAK_SIMPLE_BENCHMARK_MODULE)

#else // MANAK_SIMPLE_BENCHMARK_MODULE
#define MANAK_MODULE_NAME MANAK_STRINGIZE(MANAK_BENCHMARK_MODULE)

#endif // MANAK_SIMPLE_BENCHMARK_MODULE

//! If MANAK_ALTERNATE_INIT_FUNCTION is defined then that function will be
//! called from generated main function for initializing unit benchmarking
//! module. This function can be used manually registering cases.
#ifdef MANAK_ALTERNATE_INIT_FUNCTION
#define MANAK_INIT_FUNCTION MANAK_ALTERNATE_INIT_FUNCTION
#else
#define MANAK_INIT_FUNCTION manak::init_benchmarking_module
#endif // MANAK_ALTERNATE_INIT_FUNCTION

//! If no output file is given benchmarking statistics will be saved to this file
#ifndef MANAK_DEFAULT_OUT_FILENAME
#define MANAK_DEFAULT_OUT_FILENAME benchmark_stat.txt
#endif

//! Set default tolerance if not defined
#ifndef MANAK_DEFAULT_TOLERANCE
  #define MANAK_DEFAULT_TOLERANCE 10
#endif

//! Set default iterations if not defined
#ifndef MANAK_DEFAULT_ITERATIONS
  #define MANAK_DEFAULT_ITERATIONS 10
#endif

//! Filename where the output of benchmark cases will be stored
#ifndef MANAK_REDIRECTION_FILENAME

#define MANAK_REDIRECTION_FILENAME benchmark_log.txt

#endif // MANAK_REDIRECTION_FILENAME

//! While the benchmark is running all the output on std::cout and std::cerr
//! is redirected to another stream
//! If that stream is not defined then define it ourselves
#ifndef MANAK_BENCHMARK_REDIRECTION_STREAM

//! Open the logging file stream
#define MANAK_OPEN_LOG std::ofstream f(MANAK_STRINGIZE(MANAK_REDIRECTION_FILENAME), std::fstream::app);
#define MANAK_CLOSE_LOG f.close();
#define MANAK_BENCHMARK_REDIRECTION_STREAM f.rdbuf()

#endif

#ifndef MANAK_OPEN_LOG
#define MANAK_OPEN_LOG
#endif

#ifndef MANAK_CLOSE_LOG
#define MANAK_CLOSE_LOG
#endif

#endif // MANAK_SET_ENV_HPP_INCLUDED
