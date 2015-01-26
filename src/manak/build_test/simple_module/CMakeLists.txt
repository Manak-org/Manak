add_executable(sm_auto_reg_test auto_reg_test.cpp)

add_executable(dl_sm_auto_reg_test auto_reg_test.cpp)

target_link_libraries(dl_sm_auto_reg_test
  manak
)

set_target_properties(dl_sm_auto_reg_test PROPERTIES COMPILE_DEFINITIONS "MANAK_USE_DYN_LINK")

add_executable(sm_man_reg_test manual_reg_test.cpp)
