include_directories(..) # <manak/[whatever]>

## Recurse into both core/ and methods/.
set(DIRS
  manak_suite
  output_manager
  build_test
  test_suite
  util
)

foreach(dir ${DIRS})
    add_subdirectory(${dir})
endforeach()

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 

# the RPATH to be used when installing
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# don't add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# the RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
IF("${isSystemDir}" STREQUAL "-1")
   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
ENDIF("${isSystemDir}" STREQUAL "-1")

# MANAK_SRCS is set in the subdirectories.
# We don't use a DLL (shared) on Windows because it's a nightmare.  We can't
# easily generate the .def file and we won't put __declspec(dllexport) next to
# every function signature.
if (WIN32)
  add_library(manak ${MANAK_SRCS})
else (WIN32)
  add_library(manak SHARED ${MANAK_SRCS})
endif (WIN32)

set_target_properties(manak PROPERTIES COMPILE_FLAGS "-DMANAK_USE_DYN_LINK -DMANAK_GEN_PRECOMPILE")

# Make sure the linker can find the needed library.
# rt: clock_gettime()
if(UNIX AND NOT APPLE)
    target_link_libraries(manak rt)
endif(UNIX AND NOT APPLE)

# Collect all header files in the library.
file(GLOB_RECURSE INCLUDE_H_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h)
file(GLOB_RECURSE INCLUDE_HPP_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.hpp)
file(GLOB_RECURSE INCLUDE_HH_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.hh)
file(GLOB_RECURSE INCLUDE_IPP_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.ipp)
file(GLOB_RECURSE INCLUDE_CPP_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
set(INCLUDE_FILES ${INCLUDE_H_FILES} ${INCLUDE_HPP_FILES} ${INCLUDE_HH_FILES} ${INCLUDE_IPP_FILES} ${INCLUDE_CPP_FILES})

# Move all of these header files to <builddir>/include/libgdl/ after the library
# is built.  First we have to create that directory though.
add_custom_target(manak_headers)
add_custom_command(TARGET manak_headers POST_BUILD
  COMMENT "Moving header files to include/manak/"
  COMMAND ${CMAKE_COMMAND} ARGS -E
    make_directory ${CMAKE_BINARY_DIR}/include/manak/)
    
# Then copy each of the header files over to that directory.
foreach(incl_file ${INCLUDE_FILES})
  add_custom_command(TARGET manak_headers POST_BUILD
    COMMAND ${CMAKE_COMMAND} ARGS -E
      copy ${CMAKE_CURRENT_SOURCE_DIR}/${incl_file}
           ${CMAKE_BINARY_DIR}/include/manak/${incl_file})
endforeach()

# At install time, we simply install that directory of header files we
# collected to include/.
install(DIRECTORY ${CMAKE_BINARY_DIR}/include/manak DESTINATION include)

set_target_properties(manak
  PROPERTIES
  VERSION ${MANAK_MAJOR_VERSION}
  SOVERSION ${MANAK_MINOR_VERSION}
)

# Set generated executables to be installed.  Unfortunately they must manually
# be entered...
install(TARGETS manak
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

add_dependencies(manak manak_headers)

# For 'make test'.
add_custom_target(test
  COMMAND ${CMAKE_BINARY_DIR}/bin/sm_auto_reg_test
  COMMAND ${CMAKE_BINARY_DIR}/bin/dl_sm_auto_reg_test
  COMMAND ${CMAKE_BINARY_DIR}/bin/sm_man_reg_test
  COMMAND ${CMAKE_BINARY_DIR}/bin/dl_sm_man_reg_test
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/ # This is where test files are put.
  COMMENT "Running MANAK build test"
  DEPENDS sm_auto_reg_test 
          dl_sm_auto_reg_test
          sm_man_reg_test
          dl_sm_man_reg_test)
