target_sources(Gap PRIVATE
    tcp/server.cpp
    tcp/server.h
    tcp/client.cpp
    tcp/client.h
    tcp/socket.cpp
    tcp/socket.h
    utils/endpoint.cpp
    utils/endpoint.h
)

target_include_directories(Gap PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

install(FILES tcp/tcp_server.h DESTINATION include/net)