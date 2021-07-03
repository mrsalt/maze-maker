#pragma once
#include <vector>
#include "Primitives.h"

class Cell
{
public:
    Direction fromDirection;
    Cell(Direction f) : fromDirection(f) {}

    bool isEmpty() const { return fromDirection == Direction::EMPTY; }
};

class MazeModel
{
public:
    const Size size;
    const Location startPosition;
    const Location endPosition;
    std::vector<Cell> data;
    size_t countEmpty;

    MazeModel(Size size, Location start, Location end)
        : size(size)
        , startPosition(start)
        , endPosition(end)
    {       
        data.resize(size.width * size.height, Cell(Direction::EMPTY));
        countEmpty = data.size();
    }

    MazeModel(const MazeModel& src);

    bool inBounds(Location l) const {
        return l.x >= 0 && l.x < size.width && l.y >= 0 && l.y < size.height;
    }

    Cell & cell(Location l) {
        return data[l.y * size.width + l.x];
    }

    size_t getCountEmpty() const { return countEmpty; }

    bool canBeCompleted(Location l) const;

private:
    bool recursiveMarkToPosition(Location start, Location l);

};