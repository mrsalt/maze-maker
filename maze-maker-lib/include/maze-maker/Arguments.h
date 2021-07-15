#pragma once
#include "maze-maker/ArgParser.h"
#include <string>

class Arguments : public ArgumentParser
{
public:
    int width{ 40 };
    int height{ 30 };
    int start_x{ -1 };
    int start_y{ -1 };
    int end_x{ -1 };
    int end_y{ -1 };
    int max_path{ 10 };
    int seed{ 1 };
    int pixels_per_cell{ 30 };
    bool draw_solution{ false };
    bool draw_header{ false };
    std::string output_filename;

    Arguments()
    {
        addArgument<Type::INTEGER>("--width", "-w", "Width of maze or number of columns.", &width);
        addArgument<Type::INTEGER>("--height", "-h", "Height of maze or number of rows.", &height);
        addArgument<Type::INTEGER>("--start-x", "-sx", "Start position, x coordinate.", &start_x);
        addArgument<Type::INTEGER>("--start-y", "-sy", "Start position, y coordinate.", &start_y);
        addArgument<Type::INTEGER>("--end-x", "-ex", "End position, x coordinate.", &end_x);
        addArgument<Type::INTEGER>("--end-y", "-ey", "End position, y coordinate.", &end_y);
        addArgument<Type::INTEGER>("--max-path", "-mp", "Maximum number of cells to add in one shot.", &max_path);
        addArgument<Type::INTEGER>("--seed", "-s", "Random number seed.", &seed);
        addArgument<Type::INTEGER>("--cell-size", "-cs", "Size of each maze cell in pixels.", &pixels_per_cell);
        addArgument<Type::BOOL_FLAG>("--draw-header", "-d", "Draw header above maze showing details.", &draw_header);
        addArgument<Type::BOOL_FLAG>("--draw-solution", "-ds", "Draw solution path from start to finish.", &draw_solution);
        addArgument<Type::STRING>("--output", "-o", "Filename to write PNG file to.  (.png will be appended)", &output_filename);
    }
};