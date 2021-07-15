project(maze-maker)
cmake_minimum_required (VERSION 3.8)

find_library(CAIRO_LIB_DEBUG cairo)
find_library(CAIRO_LIB_RELEASE cairo)
find_path(CAIRO_INCLUDE_DIR cairo/cairo.h)

# Add source to this project's executable.
add_executable (maze-maker "ArgParser.h" "Arguments.h" "main.cpp" "MazeModel.cpp" "MazeModel.h" "MazeMaker.cpp" "MazeMaker.h" "Primitives.h" "Primitives.cpp" "Render.cpp")

target_include_directories(maze-maker PRIVATE ${CAIRO_INCLUDE_DIR}/cairo)
target_link_libraries(maze-maker PRIVATE $<$<CONFIG:Debug>:${CAIRO_LIB_DEBUG}> $<$<CONFIG:Release>:${CAIRO_LIB_RELEASE}>)
