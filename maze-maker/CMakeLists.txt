# CMakeList.txt : CMake project for maze-maker, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (maze-maker "maze-maker.cpp" "maze-maker.h" "MazeModel.cpp" "MazeModel.h" "MazeMaker.cpp" "MazeMaker.h" "Primitives.h")

# TODO: Add tests and install targets if needed.
