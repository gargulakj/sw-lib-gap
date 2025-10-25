add_executable(
  tmptest
  tmptest.cpp
  client_server_app.cpp
  client_server_app.h
)

target_link_libraries(tmptest
  GTest::gtest_main
  Gap
)

include(GoogleTest)
gtest_discover_tests(tmptest)
