#include <cairo.h>
#include "Render.h"
using namespace std;

bool pathIsOpen(const MazeModel& model, Location a, Location b)
{
    Direction aToB = b - a;
    Direction bToA = a - b;
    return (model.getCell(b).fromDirection == aToB) || (model.getCell(a).fromDirection == bToA);
}

void Render::output(const MazeModel& model, int pixelsPerSquare, string outputFile)
{
    cairo_surface_t* surface;
    cairo_t* cr;
    int lineThickness = 1;
    Size imageSize{ model.size.width * pixelsPerSquare + lineThickness, model.size.height * pixelsPerSquare + lineThickness };

    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, (int)imageSize.width, (int)imageSize.height);

    cr = cairo_create(surface);

    cairo_rectangle(cr, 0, 0, imageSize.width, imageSize.height);
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_fill(cr);

    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_rectangle(cr, 0, 0, imageSize.width, imageSize.height);
    cairo_stroke(cr);

    for (int y = 1; y < model.size.height; y++) {
        cairo_move_to(cr, 0, y * pixelsPerSquare);
        bool pathIsClosed = true;
        for (int x = 0; x < model.size.width; x++) {
            Location l1{ x, y - 1 };
            Location l2{ x, y };
            if (pathIsOpen(model, l1, l2)) {
                if (pathIsClosed) {
                    cairo_line_to(cr, x * pixelsPerSquare, y * pixelsPerSquare);
                    cairo_stroke(cr);
                    pathIsClosed = false;
                }
            }
            else {
                if (!pathIsClosed) {
                    cairo_move_to(cr, x * pixelsPerSquare, y * pixelsPerSquare);
                    pathIsClosed = true;
                }
            }
        }
        if (pathIsClosed) {
            cairo_line_to(cr, model.size.width * pixelsPerSquare, y * pixelsPerSquare);
            cairo_stroke(cr);
        }
    }

    for (int x = 1; x < model.size.width; x++) {
        cairo_move_to(cr, x * pixelsPerSquare, 0);
        bool pathIsClosed = true;
        for (int y = 0; y < model.size.height; y++) {
            Location l1{ x - 1, y  };
            Location l2{ x, y };
            if (pathIsOpen(model, l1, l2)) {
                if (pathIsClosed) {
                    cairo_line_to(cr, x * pixelsPerSquare, y * pixelsPerSquare);
                    cairo_stroke(cr);
                    pathIsClosed = false;
                }
            }
            else {
                if (!pathIsClosed) {
                    cairo_move_to(cr, x * pixelsPerSquare, y * pixelsPerSquare);
                    pathIsClosed = true;
                }
            }
        }
        if (pathIsClosed) {
            cairo_line_to(cr, x * pixelsPerSquare, model.size.height * pixelsPerSquare);
            cairo_stroke(cr);
        }
    }

    cairo_surface_write_to_png(surface, outputFile.c_str());
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}
