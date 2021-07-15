find_library(CAIRO_LIB_DEBUG cairo)
find_library(CAIRO_LIB_RELEASE cairo)
find_path(CAIRO_INCLUDE_DIR cairo/cairo.h)

file(GLOB HEADER_LIST CONFIGURE_DEPENDS "include/maze-maker/*.h")
set(SRC_LIST
    "src/MazeModel.cpp"
    "src/MazeMaker.cpp"
    "src/Primitives.cpp"
    "src/Render.cpp"
)

add_library (maze-lib ${SRC_LIST} ${HEADER_LIST})

# We need this directory, and users of our library will need it too
target_include_directories(maze-lib PUBLIC include)

target_include_directories(maze-lib PRIVATE ${CAIRO_INCLUDE_DIR}/cairo)
target_link_libraries(maze-lib PRIVATE $<$<CONFIG:Debug>:${CAIRO_LIB_DEBUG}> $<$<CONFIG:Release>:${CAIRO_LIB_RELEASE}>)
