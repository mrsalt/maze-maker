#pragma once
#include "MazeModel.h"
#include <string>

class Render
{
public:
    static void output(const MazeModel & model, int pixelsPerSquare, std::string outputFile);
};