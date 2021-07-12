#include "Arguments.h"
#include "MazeModel.h"
#include "MazeMaker.h"
#include "Render.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    Arguments args;
    if (!args.parse(argc, argv))
    {
        return -1;
    }

    try {
        MazeModel model({ (size_t)args.width, (size_t)args.height }, { args.start_x, args.start_y }, { args.end_x, args.end_y });
        MazeMaker maker(model);
        maker.go(args.seed, args.max_path);
        Render::output(model, args);
    }
    catch (exception e) {
        cout << "Exception occurred: " << e.what() << endl;
    }
    return 0;
}
