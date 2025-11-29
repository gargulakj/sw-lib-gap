target_sources(Gap PRIVATE
    tcp/server.cpp
    tcp/server.h
    tcp/connection.cpp
    tcp/connection.h
    tcp/socket.cpp
    tcp/socket.h
    utils/endpoint.cpp
    utils/endpoint.h
    utils/buffer.cpp
    utils/buffer.h
)

target_include_directories(Gap PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

install(FILES tcp/server.h DESTINATION include/net)
install(FILES tcp/connection.h DESTINATION include/net)
install(FILES tcp/socket.h DESTINATION include/net)
install(FILES utils/endpoint.h DESTINATION include/net)
install(FILES utils/buffer.h DESTINATION include/net)