cmake_minimum_required(VERSION 3.1)

project(space)

## If you want to link SFML statically
# set(SFML_STATIC_LIBRARIES TRUE)

file(GLOB src_files
    "src/*.cpp"
    "src/effects/*.cpp"
    "src/game/*.cpp"
    "src/game/items/*.cpp"
    "src/game/interactions/*.cpp"
    "src/debug/*.cpp"
    "src/definitions/*.cpp"
    "src/serialisers/json/*.cpp"
    "src/physics/*.cpp"
    "src/imgui/*.cpp"
    "src/ui/*.cpp"
    "src/game-ui/*.cpp"
    "src/game-ui/in-game/*.cpp"
    "src/game-ui/main-menu/*.cpp"
    "src/controllers/*.cpp"
    "src/controllers/actions/*.cpp"
)

SET (CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -DTRACK_MEMORY -Wall")
SET (CMAKE_CXX_FLAGS_DEBUG "-g -DTRACK_MEMORY -Wall")
#SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wconversion-null")

set("OpenGL_GL_PREFERENCE" GLVND)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

add_subdirectory("extern/SFML")
add_subdirectory("extern/tmxlite/tmxlite")
add_subdirectory("extern/yoga")
add_subdirectory("extern/Observe")
add_subdirectory("extern/box2d")
add_subdirectory("extern/glm")

SET (SFML_LIBRARIES sfml-graphics sfml-audio sfml-window sfml-system)
SET (BASE_LIBRARIES vorbis vorbisenc vorbisfile ${OPENGL_LIBRARIES} ${SFML_LIBRARIES} box2d tmxlite X11 yoga Observe vorbis glm)

if( MAKE_TESTS )
    find_package(doctest REQUIRED)

    add_executable(space_tests tests/main.cpp ${src_files})
    target_link_libraries(space_tests ${BASE_LIBRARIES} doctest::doctest)
    target_include_directories(space_tests PRIVATE src)
    target_compile_features(space_tests PRIVATE cxx_std_17)
else()
    add_executable(space main.cpp ${src_files})
    target_link_libraries(space ${BASE_LIBRARIES})
    target_include_directories(space PRIVATE src)
    target_compile_features(space PRIVATE cxx_std_17)
endif()