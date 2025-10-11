target_sources(Gap PRIVATE
    process.cpp
    process.h
)

target_include_directories(Gap INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

install(FILES process.h DESTINATION include/process)