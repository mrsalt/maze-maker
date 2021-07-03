// maze-maker.cpp : Defines the entry point for the application.
//

#include "maze-maker.h"
#include "MazeModel.h"
#include "MazeMaker.h"

using namespace std;

int main()
{
    MazeModel model({ 40, 20 }, { 0, 0 }, { 39, 19 });
    MazeMaker maker(model);

    while (model.getCountEmpty() > 0) {
        
    }

	return 0;
}
