#include "MazeModel.h"
#include "MazeMaker.h"
#include <iostream>

using namespace std;

int main()
{
    try {
        MazeModel model({ 40, 20 }, { 0, 0 }, { 39, 19 });
        MazeMaker maker(model);
        maker.go();
    }
    catch (exception e) {
        cout << "Exception occurred: " << e.what() << endl;
    }
    return 0;
}
