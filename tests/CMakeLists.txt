add_executable(
  gaptesting
  client_server_app.cpp
  client_server_app.h
)

target_link_libraries(gaptesting
  GTest::gtest_main
  Gap
)

include(GoogleTest)
gtest_discover_tests(gaptesting)
