#include <cairo.h>
#include <algorithm>
#include "maze-maker/Render.h"
using namespace std;

void Render::output(const MazeModel& model, const Arguments & args)
{
    int pixelsPerSquare = args.pixels_per_cell;
    string outputFile;
    if (args.output_filename.empty()) {
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "maze-%d-%dx%d%s.png", args.seed, args.width, args.height, args.draw_solution ? "-solution" : "");
        outputFile = buffer;
    }
    else {
        outputFile = args.output_filename + ".png";
    }
    cairo_surface_t* surface;
    cairo_t* cr;
    int lineThickness = 1;
    Size imageSize{ model.size.width * pixelsPerSquare + lineThickness, model.size.height * pixelsPerSquare + lineThickness };

    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, (int)imageSize.width, (int)imageSize.height);

    cr = cairo_create(surface);
    cairo_set_line_width(cr, lineThickness);

    renderMaze(cr, imageSize, model, pixelsPerSquare);

    if (args.draw_solution) {
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
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}

void Render::renderMaze(cairo_t* cr, const Size& imageSize, const MazeModel& model, int pixelsPerSquare)
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
    drawHorizontalLine(cr, 0, model, pixelsPerSquare, isHoEdgeOpen);
    drawHorizontalLine(cr, model.size.height, model, pixelsPerSquare, isHoEdgeOpen);
    drawVerticalLine(cr, 0, model, pixelsPerSquare, isVeEdgeOpen);
    drawVerticalLine(cr, model.size.width, model, pixelsPerSquare, isVeEdgeOpen);
    cairo_stroke(cr);

    // draw the innards of the maze
    function<bool(const MazeModel&, Location, Location)> isOpen = [](const MazeModel& m, Location l1, Location l2) { return m.pathIsOpen(l1, l2); };
    for (int y = 1; y < model.size.height; y++) {
        drawHorizontalLine(cr, y, model, pixelsPerSquare, isOpen);
    }
    for (int x = 1; x < model.size.width; x++) {
        drawVerticalLine(cr, x, model, pixelsPerSquare, isOpen);
    }
}

void Render::drawHorizontalLine(cairo_t * cr, int y, const MazeModel & model, int pixelsPerSquare, function<bool(const MazeModel&, Location, Location)> isOpen)
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

void Render::drawVerticalLine(cairo_t* cr, int x, const MazeModel& model, int pixelsPerSquare, std::function<bool(const MazeModel&, Location, Location)> isOpen)
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
