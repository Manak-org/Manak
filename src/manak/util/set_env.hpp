#ifndef MANAK_SET_ENV_HPP_INCLUDED
#define MANAK_SET_ENV_HPP_INCLUDED

#include <fstream>

#include "macro_utils.hpp"

//! Set the module name for further use
#ifdef MANAK_SIMPLE_MODULE
#define MANAK_MODULE_NAME MANAK_STRINGIZE(MANAK_SIMPLE_MODULE)

#else // MANAK_SIMPLE_MODULE
#define MANAK_MODULE_NAME MANAK_STRINGIZE(MANAK_MODULE)

#endif // MANAK_SIMPLE_BENCHMARK_MODULE

//! If no output file is given benchmarking statistics will be saved to this file
#ifndef MANAK_DEFAULT_OUT_FILENAME
#define MANAK_DEFAULT_OUT_FILENAME benchmark_stat
#endif

//! Set default tolerance if not defined
#ifndef MANAK_DEFAULT_TOLERANCE
  #define MANAK_DEFAULT_TOLERANCE 10
#endif

//! Set default iterations if not defined
#ifndef MANAK_DEFAULT_ITERATIONS
  #define MANAK_DEFAULT_ITERATIONS 10
#endif

//! Set default success percentage for benchmark-test cases
#ifndef MANAK_DEFAULT_SP
  #define MANAK_DEFAULT_SP 100
#endif // MANAK_DEFAULT_SP

//! While the benchmark is running all the output on std::cout and std::cerr
//! is redirected to another stream
//! if MANAK_REDIRECTION_FILENAME is set the output is redirected to that file
#ifndef MANAK_REDIRECTION_FILENAME

#define MANAK_REDIRECTION_FILENAME benchmark_log.txt

#endif // MANAK_REDIRECTION_FILENAME

//! MANAK_REDIRECTION_BUFFER is the buffer to which std::cout and std::cerr is
//! redirected while benchmark is running. If its not defined it is redirected
//! to file given by MANAK_REDIRECTION_FILENAME
#ifndef MANAK_REDIRECTION_BUFFER

//! Open the logging file stream
#define MANAK_OPEN_LOG std::ofstream f(MANAK_STRINGIZE(MANAK_REDIRECTION_FILENAME), std::fstream::app);
#define MANAK_CLOSE_LOG f.close();
#define MANAK_REDIRECTION_BUFFER f.rdbuf()

#endif //MANAK_REDIRECTION_BUFFER

#ifndef MANAK_OPEN_LOG
#define MANAK_OPEN_LOG
#endif

#ifndef MANAK_CLOSE_LOG
#define MANAK_CLOSE_LOG
#endif

#endif // MANAK_SET_ENV_HPP_INCLUDED
