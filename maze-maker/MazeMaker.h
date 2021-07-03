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

    bool addPath(Direction direction, int distance);

};