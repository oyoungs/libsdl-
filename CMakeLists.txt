cmake_minimum_required(VERSION 2.8)

project(SDL)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "-g -pthread ${CMAKE_CXX_FLAGS}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    )

add_library(sdl++
    src/sdl++.cc
    src/sdl++-mixer.cc
    )


target_link_libraries(sdl++ SDL2 SDL2_mixer)

add_executable(sdl-cli
   examples/main.cc)
target_link_libraries(sdl-cli sdl++)


install(TARGETS sdl++
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    )

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/sdl++
    DESTINATION include
    )
