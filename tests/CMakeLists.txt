add_executable(
  gaptest-client-server
  client_server_app.cpp
  client_server_app.h
)

add_executable(
  gaptest-signal-handler
  signal_handler.cpp
)

target_link_libraries(gaptest-client-server
  GTest::gtest_main
  Gap
)

target_link_libraries(gaptest-signal-handler
  GTest::gtest_main
  Gap
)

include(GoogleTest)
gtest_discover_tests(gaptest-client-server)
gtest_discover_tests(gaptest-signal-handler)
