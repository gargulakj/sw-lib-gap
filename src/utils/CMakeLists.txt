target_sources(Gap PRIVATE
    flags.h
)

target_include_directories(Gap PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

install(FILES flags.h DESTINATION include/utils)