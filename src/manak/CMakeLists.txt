include_directories(..) # <manak/[whatever]>

# Collect all header files in the library.
file(GLOB_RECURSE INCLUDE_H_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h)
file(GLOB_RECURSE INCLUDE_HPP_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.hpp)
file(GLOB_RECURSE INCLUDE_HH_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.hh)
set(INCLUDE_FILES ${INCLUDE_H_FILES} ${INCLUDE_HPP_FILES} ${INCLUDE_HH_FILES})

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

# LIBGDL_SRCS is set in the subdirectories.
add_library(manak OBJECT ${INCLUDE_FILES})
set_target_properties(manak PROPERTIES LINKER_LANGUAGE CXX)

set_target_properties(manak
  PROPERTIES
  VERSION ${LIBGDL_MAJOR_VERSION}
  SOVERSION ${LIBGDL_MINOR_VERSION}
)
  
add_dependencies(manak manak_headers)

add_subdirectory(build_test)

# For 'make test'.
add_custom_target(test
  COMMAND ${CMAKE_BINARY_DIR}/bin/sm_auto_reg_test
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/ # This is where test files are put.
  COMMENT "Running MANAK build test"
  DEPENDS sm_auto_reg_test)
