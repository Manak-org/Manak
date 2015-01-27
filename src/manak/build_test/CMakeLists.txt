add_subdirectory(simple_module)
add_subdirectory(module)

add_executable(mfile_test 
mfile_test.cpp
mfile_test_1.cpp
mfile_test_2.cpp
)

add_executable(dl_mfile_test
mfile_test.cpp
mfile_test_1.cpp
mfile_test_2.cpp
)

target_link_libraries(dl_mfile_test
  manak
)

set_target_properties(dl_mfile_test PROPERTIES COMPILE_DEFINITIONS "MANAK_USE_DYN_LINK")
