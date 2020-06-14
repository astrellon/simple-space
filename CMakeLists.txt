cmake_minimum_required(VERSION 3.1)

project(space)

## If you want to link SFML statically
# set(SFML_STATIC_LIBRARIES TRUE)

file(GLOB src_files
    "src/*.cpp"
    "src/game/*.cpp"
    "src/debug/*.cpp"
    "src/definitions/*.cpp"
    "src/serialisers/json/*.cpp"
)

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

add_executable(space main.cpp ${src_files})
target_link_libraries(space sfml-graphics sfml-audio sfml-window sfml-system)
target_include_directories(space PRIVATE src)
target_compile_features(space PRIVATE cxx_std_17)
