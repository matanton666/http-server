# tests/CMakeLists.txt


file(GLOB TEST_SOURCES "*.c")
foreach(TEST_SOURCE ${TEST_SOURCES})
  # make address sanatizer wait for end of execution (so can see tests results if there is address error)
  set_tests_properties(${TEST_NAME} PROPERTIES ENVIRONMENT "ASAN_OPTIONS=halt_on_error=0")
  get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
  add_executable(${TEST_NAME} ${TEST_SOURCE})
  target_link_libraries(${TEST_NAME} PRIVATE unity ${PROJECT_NAME}_lib)
  # target_link_libraries(${TEST_NAME}  ${PROJECT_NAME}_lib)
  add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
  
endforeach()

