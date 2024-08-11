# tests/CMakeLists.txt


file(GLOB TEST_SOURCES "*.c")
foreach(TEST_SOURCE ${TEST_SOURCES})
  get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
  add_executable(${TEST_NAME} ${TEST_SOURCE})
  target_link_libraries(${TEST_NAME} ${PROJECT_NAME}_lib)
  add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()