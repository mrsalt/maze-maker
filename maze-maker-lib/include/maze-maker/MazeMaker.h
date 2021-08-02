#pragma once
#include "maze-maker/MazeModel.h"
#include "maze-maker/Primitives.h"

enum class MazeState
{
    NotStarted,
    FindingPathToEnd,
    FillingRuses,
    Complete,
};

class MazeMaker
{
public:
    MazeModel& model;
    Location position;
    const int maxDistance;
    MazeState state;

    MazeMaker(MazeModel& model, int seed, int maxDistance)
    : model(model)
    , position(model.startPosition)
    , maxDistance(maxDistance)
    , state(MazeState::NotStarted)
    {
        srand(seed);
    }

    void go();
    bool iterate(); // returns true if maze is not complete

protected:
    int addPath(Direction direction, int distance);
    void unwindPath(Direction direction, int distance);
    size_t countyEmptyCellsWithAdjacentOccupiedCells(int y) const;
    Location findBranch() const;
};