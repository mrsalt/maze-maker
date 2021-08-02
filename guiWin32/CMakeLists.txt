add_executable (maze-gui WIN32 "maze.cpp")

target_link_libraries(maze-gui PRIVATE maze-lib)