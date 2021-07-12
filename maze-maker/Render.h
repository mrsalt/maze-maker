#pragma once
#include "MazeModel.h"
#include "Arguments.h"
#include <string>

class Render
{
public:
    static void output(const MazeModel & model, const Arguments &);
};