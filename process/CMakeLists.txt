target_sources(Gaf PRIVATE
    process.cpp
    process.h
)

target_include_directories(Gaf INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

install(FILES process.h DESTINATION include)