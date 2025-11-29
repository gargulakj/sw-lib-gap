add_executable( gaptest-client-server
  tcp/client_server_app.cpp
  tcp/client_server_app.h
)

add_executable( gaptest-signal-handler
  process/signal_handler.cpp
)

add_executable( gaptest-utils-buffer
  utils/buffer.cpp
)


target_link_libraries(gaptest-client-server
  GTest::gtest_main
  Gap
)

target_link_libraries(gaptest-signal-handler
  GTest::gtest_main
  Gap
)

target_link_libraries(gaptest-utils-buffer
  GTest::gtest_main
  Gap
)

include(GoogleTest)
gtest_discover_tests(gaptest-client-server)
gtest_discover_tests(gaptest-signal-handler)
gtest_discover_tests(gaptest-utils-buffer)
