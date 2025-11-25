target_sources(Gap PRIVATE
    process.cpp
    process.h
    event_provider.cpp
    event_provider.h
    event_consumer.h
    signal_handler.cpp
    signal_handler.h
)

target_include_directories(Gap PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

install(FILES process.h DESTINATION include/process)
install(FILES event_provider.h DESTINATION include/process)
install(FILES event_consumer.h DESTINATION include/process)
install(FILES signal_handler.h DESTINATION include/process)