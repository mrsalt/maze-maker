#include "maze-maker/MazeApp.h"

MazeApp::MazeApp(Size size, Location start, Location end, int seed, int maxDistance, int pixelsPerCell)
    : model(size, start, end, this)
    , maker(model, seed, maxDistance)
    , renderer(model, pixelsPerCell)
{
}

bool MazeApp::iterate()
{
    return maker.iterate();
}
