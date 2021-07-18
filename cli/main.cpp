#include "Arguments.h"
#include "maze-maker/MazeModel.h"
#include "maze-maker/MazeMaker.h"
#include "maze-maker/MazeRenderer.h"
#include <iostream>
#include <chrono>

using namespace std;

int main(int argc, char** argv)
{
    Arguments args;
    if (!args.parse(argc, argv)) {
        return -1;
    }

    try {
        if (args.start_x == -1)
            args.start_x = 0;
        if (args.start_y == -1)
            args.start_y = 0;
        if (args.end_x == -1)
            args.end_x = args.width - 1;
        if (args.end_y == -1)
            args.end_y = args.height - 1;
        MazeModel model({ (size_t)args.width, (size_t)args.height }, { args.start_x, args.start_y }, { args.end_x, args.end_y });
        if (!model.inBounds(model.startPosition)) {
            cout << "Error: maze start position {" << model.startPosition.x << ", " << model.startPosition.y << "} is not inside maze boundaries.\n";
            return 1;
        }
        if (!model.inBounds(model.endPosition)) {
            cout << "Error: maze end position {" << model.endPosition.x << ", " << model.endPosition.y << "} is not inside maze boundaries.\n";
            return 1;
        }
        MazeMaker maker(model);
        auto start = chrono::steady_clock::now();
        maker.go(args.seed, args.max_path);
        auto end = chrono::steady_clock::now();
        chrono::duration<double> elapsed_seconds = end - start;
        cout << "Maze generated in " << elapsed_seconds.count() << "s\n";

        start = end;

        string outputFile;
        if (args.output_filename.empty()) {
            char buffer[64];
            snprintf(buffer, sizeof(buffer), "maze-%d-%dx%d%s.png", args.seed, args.width, args.height, args.draw_solution ? "-solution" : "");
            outputFile = buffer;
        }
        else {
            outputFile = args.output_filename + ".png";
        }
        MazeRenderer renderer(model, args.pixels_per_cell);
        renderer.output(outputFile, args.draw_solution);
        end = chrono::steady_clock::now();
        elapsed_seconds = end - start;
        cout << "Maze .png output in " << elapsed_seconds.count() << "s\n";
    }
    catch (exception e) {
        cout << "Exception occurred: " << e.what() << endl;
    }
    return 0;
}
