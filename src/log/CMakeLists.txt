target_sources(Gap PRIVATE
    log.cpp
    log.h
)

target_include_directories(Gap PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

install(FILES log.h DESTINATION include/log)
