cmake_minimum_required(VERSION 3.0)
project(yoga)
SET(PROJECT_NAME yoga)

SET (CMAKE_CXX_FLAGS_RELEASE "-O2 -fno-omit-frame-pointer -fexceptions -fvisibility=hidden -ffunction-sections -fdata-sections -Wall -Werror -std=c++11 -DYG_ENABLE_EVENTS")

SET (CMAKE_CXX_FLAGS_RELEASE "-g -fno-omit-frame-pointer -fexceptions -fvisibility=hidden -ffunction-sections -fdata-sections -Wall -Werror -std=c++11 -DYG_ENABLE_EVENTS")

file(GLOB src_files
    "yoga/*.cpp"
    "yoga/event/*.cpp"
    "yoga/internal/*.cpp"
)

include_directories(.)

add_library(yoga SHARED ${src_files})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})