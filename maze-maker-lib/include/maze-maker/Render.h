#pragma once
#include "maze-maker/MazeModel.h"
#include "maze-maker/Arguments.h"
#include <string>
#include <functional>

typedef struct _cairo cairo_t;

class Render
{
public:
    static void output(const MazeModel & model, const Arguments &);
private:
    static void renderMaze(cairo_t* cr, const Size& imageSize, const MazeModel& model, int pixelsPerSquare);
    static void drawHorizontalLine(cairo_t* cr, int y, const MazeModel& model, int pixelsPerSquare, std::function<bool(const MazeModel&, Location, Location)> isOpen);
    static void drawVerticalLine(cairo_t* cr, int x, const MazeModel& model, int pixelsPerSquare, std::function<bool(const MazeModel&, Location, Location)> isOpen);
};