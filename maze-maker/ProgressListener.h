#pragma once
#include "Primitives.h"

class ProgressListener
{
public:
    virtual void onCellChanged(Location l) = 0;
};