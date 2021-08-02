#pragma once
#include "maze-maker/MazeModel.h"
#include <string>
#include <functional>

typedef struct _cairo cairo_t;
typedef struct _cairo_surface cairo_surface_t;

class MazeRenderer
{
    cairo_t* cr;
    cairo_surface_t* surface;
    const MazeModel& model;
    int lineThickness;
    int pixelsPerSquare;
    const Size imageSize;
public:
    MazeRenderer(const MazeModel& model, int pixelsPerSquare);
    ~MazeRenderer();
    void render(bool drawSolution);
    void output(const std::string& output_filename) const;
    Size getImageSize() const { return imageSize; }
    unsigned char* getImageBits() const;
    int getImageStride() const;
private:
    void renderMaze();
    void renderEmptyCells();
    void drawHorizontalLine(int y, std::function<bool(const MazeModel&, Location, Location)> isOpen);
    void drawVerticalLine(int x, std::function<bool(const MazeModel&, Location, Location)> isOpen);
};