#include "MazeModel.h"
#include "MazeMaker.h"
#include "Render.h"
#include <iostream>

using namespace std;

int main()
{
    try {
        MazeModel model({ 80, 60 }, { 0, 0 }, { 79, 59 });
        MazeMaker maker(model);
        maker.go();
        Render::output(model, 50, "maze.png");
    }
    catch (exception e) {
        cout << "Exception occurred: " << e.what() << endl;
    }
    return 0;
}
