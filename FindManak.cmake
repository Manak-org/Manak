# - Find Manak
# Find the Manak C++ Benchmarking Library
#
# Using this function:
#  find_package(Manak REQUIRED)
#  include_directories(${MANAK_INCLUDE_DIRS})
#  add_executable(foo foo.cc)
# 
# This module sets the following variables:
#  MANAK_FOUND - set to true if the library is found
#  MANAK_INCLUDE_DIR - list of required include directories
#  MANAK_VERSION_MAJOR - major version number
#  MANAK_VERSION_MINOR - minor version number
#  MANAK_VERSION_PATCH - patch version number
#  MANAK_VERSION_STRING - version number as a string (ex: "1.0.4")

find_path(MANAK_INCLUDE_DIR manak.hpp
          PATH_SUFFIXES manak)
  
if(MANAK_INCLUDE_DIR)

  # ------------------------------------------------------------------------
  #  Extract version information
  # ------------------------------------------------------------------------

  if(EXISTS "${MANAK_INCLUDE_DIR}/util/version.hpp")

    # Read and parse version header file for version number
    file(READ "${MANAK_INCLUDE_DIR}/util/version.hpp" _MANAK_HEADER_CONTENTS)
    string(REGEX REPLACE ".*#define __MANAK_VERSION_MAJOR ([0-9]+).*" "\\1" MANAK_VERSION_MAJOR "${_MANAK_HEADER_CONTENTS}")
    string(REGEX REPLACE ".*#define __MANAK_VERSION_MINOR ([0-9]+).*" "\\1" MANAK_VERSION_MINOR "${_MANAK_HEADER_CONTENTS}")
    string(REGEX REPLACE ".*#define __MANAK_VERSION_PATCH ([0-9]+).*" "\\1" MANAK_VERSION_PATCH "${_MANAK_HEADER_CONTENTS}")

  endif(EXISTS "${MANAK_INCLUDE_DIR}/util/version.hpp")

  set(MANAK_VERSION_STRING "${MANAK_VERSION_MAJOR}.${MANAK_VERSION_MINOR}.${MANAK_VERSION_PATCH}")
endif (MANAK_INCLUDE_DIR)


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Manak  DEFAULT_MSG
                                  MANAK_INCLUDE_DIR)

if (MANAK_FOUND)
  message(STATUS "MANAK Version: ${MANAK_VERSION_STRING}")
endif (MANAK_FOUND)
