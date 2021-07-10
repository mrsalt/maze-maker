#pragma once
#include "MazeModel.h"
#include "Primitives.h"

class MazeMaker
{
public:
    MazeModel& model;
    Location position;

    MazeMaker(MazeModel& model)
    : model(model)
    {
        position = model.startPosition;
    }

    void go();

protected:
    int addPath(Direction direction, int distance);
    void unwindPath(Direction direction, int distance);
    size_t countyEmptyCellsWithAdjacentOccupiedCells(int y) const;
    Location findBranch() const;
};