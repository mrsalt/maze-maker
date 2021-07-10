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
    std::vector<size_t> emptyCountByRow;
    size_t countEmpty;

    MazeModel(Size size, Location start, Location end);

    MazeModel(const MazeModel& src);

    bool inBounds(Location l) const {
        return l.x >= 0 && l.x < size.width && l.y >= 0 && l.y < size.height;
    }

    size_t getCountEmpty() const { return countEmpty; }

    bool canBeCompleted(Location l) const;

    const Cell& getCell(Location l) const {
        return data[l.y * size.width + l.x];
    }

    size_t nonEmptyNeighbors(Location l) const;
    size_t emptyNeighbors(Location l) const;

    Location pickNeighbor(Location l, int rand_value) const;

    int addPath(Direction direction, int distance);

    void unwindPath(Direction direction, int distance);

    bool markCell(Location l, Direction d);

private:
    Cell& cell(Location l) {
        return data[l.y * size.width + l.x];
    }

    bool recursiveMarkToPosition(Location start, Location l);

};