project(maze-maker)
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (maze-maker "main.cpp" "MazeModel.cpp" "MazeModel.h" "MazeMaker.cpp" "MazeMaker.h" "Primitives.h" "Primitives.cpp")

# TODO: Add tests and install targets if needed.
