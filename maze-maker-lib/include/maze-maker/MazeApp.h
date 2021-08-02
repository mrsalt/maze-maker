#include "maze-maker/MazeModel.h"
#include "maze-maker/MazeMaker.h"
#include "maze-maker/MazeRenderer.h"

class MazeApp : protected ProgressListener
{
public:
    MazeApp(Size size, Location start, Location end, int seed, int maxDistance, int pixelsPerCell);
    bool iterate();
    const MazeModel& getModel() { return model; }
    MazeRenderer& getRenderer() { return renderer; }

protected:
    virtual void onCellChanged(Location l) = 0;
    virtual void onPathAdded(Location begin, Location end) = 0;

    MazeModel model;
    MazeMaker maker;
    MazeRenderer renderer;
};