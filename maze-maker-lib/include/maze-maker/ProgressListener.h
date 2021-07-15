#pragma once
#include "maze-maker/Primitives.h"

class ProgressListener
{
public:
    virtual void onCellChanged(Location l) = 0;
};