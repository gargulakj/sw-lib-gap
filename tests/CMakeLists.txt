add_executable(
  tmptest
  tmptest.cpp
)

target_link_libraries(tmptest
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tmptest)
