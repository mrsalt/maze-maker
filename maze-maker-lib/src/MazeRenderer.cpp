#include <cairo.h>
#include <algorithm>
#include "maze-maker/MazeRenderer.h"
using namespace std;

MazeRenderer::MazeRenderer(const MazeModel& model, int pixelsPerSquare)
    : model(model)
    , lineThickness{1}
    , pixelsPerSquare(pixelsPerSquare)
    , imageSize{ model.size.width * pixelsPerSquare + lineThickness, model.size.height * pixelsPerSquare + lineThickness }
{
    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, (int)imageSize.width, (int)imageSize.height);
    cr = cairo_create(surface);
}

MazeRenderer::~MazeRenderer()
{
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}

void MazeRenderer::output(const string & outputFile, bool drawSolution)
{
    cairo_set_line_width(cr, lineThickness);

    renderMaze();

    if (drawSolution) {
        Location p = model.endPosition;
        cairo_set_source_rgb(cr, 1.0, 0, 0);
        cairo_move_to(cr, (p.x + 0.5) * pixelsPerSquare + 0.5, (p.y + 0.5) * pixelsPerSquare + 0.5);
        do {
            p = p - model.getCell(p).fromDirection;
            cairo_line_to(cr, (p.x + 0.5) * pixelsPerSquare + 0.5, (p.y + 0.5) * pixelsPerSquare + 0.5);
        } while (p != model.startPosition);
        cairo_stroke(cr);
    }

    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_font_size(cr, pixelsPerSquare / 3.0);
    cairo_move_to(cr, (model.startPosition.x + 0.1) * pixelsPerSquare, (model.startPosition.y + 0.6) * pixelsPerSquare);
    cairo_show_text(cr, "Start");

    cairo_move_to(cr, (model.endPosition.x + 0.2) * pixelsPerSquare, (model.endPosition.y + 0.6) * pixelsPerSquare);
    cairo_show_text(cr, "End");

    cairo_surface_write_to_png(surface, outputFile.c_str());
}

void MazeRenderer::renderMaze()
{
    // make background white
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_rectangle(cr, 0, 0, imageSize.width, imageSize.height);
    cairo_fill(cr);

    // draw maze frame
    function<bool(const MazeModel&, Location, Location, bool)> isEdgeOpen = [](const MazeModel& m, Location l1, Location l2, bool horizontal) {
        for (Location l : {l1, l2}) {
            if (l == m.startPosition || l == m.endPosition) {
                if (m.isCorner(l)) {
                    // return true if opposite edge is closed
                    Location neighbor;
                    if (horizontal)
                        neighbor = l + (l.y == 0 ? Direction::DOWN : Direction::UP);
                    else
                        neighbor = l + (l.x == 0 ? Direction::RIGHT : Direction::LEFT);
                    return m.pathIsOpen(l, neighbor);
                }
                return true;
            }
        }
        return false;
    };
    auto isHoEdgeOpen = bind(isEdgeOpen, placeholders::_1, placeholders::_2, placeholders::_3, true);
    auto isVeEdgeOpen = bind(isEdgeOpen, placeholders::_1, placeholders::_2, placeholders::_3, false);

    cairo_set_source_rgb(cr, 0, 0, 0);
    drawHorizontalLine(0, isHoEdgeOpen);
    drawHorizontalLine(model.size.height,isHoEdgeOpen);
    drawVerticalLine(0, isVeEdgeOpen);
    drawVerticalLine(model.size.width, isVeEdgeOpen);
    cairo_stroke(cr);

    // draw the innards of the maze
    function<bool(const MazeModel&, Location, Location)> isOpen = [](const MazeModel& m, Location l1, Location l2) { return m.pathIsOpen(l1, l2); };
    for (int y = 1; y < model.size.height; y++) {
        drawHorizontalLine(y, isOpen);
    }
    for (int x = 1; x < model.size.width; x++) {
        drawVerticalLine(x, isOpen);
    }
}

void MazeRenderer::drawHorizontalLine(int y, function<bool(const MazeModel&, Location, Location)> isOpen)
{
    cairo_move_to(cr, 0.5, y * pixelsPerSquare + 0.5);
    bool pathIsClosed = true;
    for (int x = 0; x < model.size.width; x++) {
        Location l1{ x, y - 1 };
        Location l2{ x, y };
        if (isOpen(model, l1, l2)) {
            if (pathIsClosed) {
                cairo_line_to(cr, x * pixelsPerSquare + 0.5, y * pixelsPerSquare + 0.5);
                cairo_stroke(cr);
                pathIsClosed = false;
            }
        }
        else {
            if (!pathIsClosed) {
                cairo_move_to(cr, x * pixelsPerSquare + 0.5, y * pixelsPerSquare + 0.5);
                pathIsClosed = true;
            }
        }
    }
    if (pathIsClosed) {
        cairo_line_to(cr, model.size.width * pixelsPerSquare + 0.5, y * pixelsPerSquare + 0.5);
        cairo_stroke(cr);
    }
}

void MazeRenderer::drawVerticalLine(int x, function<bool(const MazeModel&, Location, Location)> isOpen)
{
    cairo_move_to(cr, x * pixelsPerSquare + 0.5, 0.5);
    bool pathIsClosed = true;
    for (int y = 0; y < model.size.height; y++) {
        Location l1{ x - 1, y };
        Location l2{ x, y };
        if (isOpen(model, l1, l2)) {
            if (pathIsClosed) {
                cairo_line_to(cr, x * pixelsPerSquare + 0.5, y * pixelsPerSquare + 0.5);
                cairo_stroke(cr);
                pathIsClosed = false;
            }
        }
        else {
            if (!pathIsClosed) {
                cairo_move_to(cr, x * pixelsPerSquare + 0.5, y * pixelsPerSquare + 0.5);
                pathIsClosed = true;
            }
        }
    }
    if (pathIsClosed) {
        cairo_line_to(cr, x * pixelsPerSquare + 0.5, model.size.height * pixelsPerSquare + 0.5);
        cairo_stroke(cr);
    }
}
