cmake_minimum_required(VERSION 3.1)

project(space)

## If you want to link SFML statically
# set(SFML_STATIC_LIBRARIES TRUE)

file(GLOB src_files
    "src/*.cpp"
    "src/effects/*.cpp"
    "src/game/*.cpp"
    "src/game/items/*.cpp"
    "src/debug/*.cpp"
    "src/definitions/*.cpp"
    "src/serialisers/json/*.cpp"
    "src/physics/*.cpp"
    "src/imgui/*.cpp"
    "src/ui/*.cpp"
)

SET (CMAKE_CXX_FLAGS_RELEASE_INIT "-O3 -DNDEBUG")
#SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wconversion-null")

set("OpenGL_GL_PREFERENCE" GLVND)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

add_subdirectory("extern/tmxlite/tmxlite")



add_executable(space main.cpp ${src_files})
target_link_libraries(space ${OPENGL_LIBRARIES} sfml-graphics sfml-audio sfml-window sfml-system Box2D tmxlite)
target_include_directories(space PRIVATE src)
target_compile_features(space PRIVATE cxx_std_17)
