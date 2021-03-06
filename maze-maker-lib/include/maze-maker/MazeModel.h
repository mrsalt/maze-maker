#pragma once
#include <vector>
#include "maze-maker/Primitives.h"
#include "maze-maker/ProgressListener.h"

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
    ProgressListener* listener;

    MazeModel(Size size, Location start, Location end, ProgressListener * listener = nullptr);

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

    bool pathIsOpen(Location a, Location b) const {
        Direction aToB = b - a;
        Direction bToA = a - b;
        return (getCell(b).fromDirection == aToB) || (getCell(a).fromDirection == bToA);
    }

    bool isCorner(Location l) const {
        return (l.x == 0 && (l.y == 0 || l.y == size.height - 1)) ||
               (l.x == size.width - 1 && (l.y == 0 || l.y == size.height - 1));
    }

    int addPath(Direction direction, int distance);

    void unwindPath(Direction direction, int distance);

    bool markCell(Location l, Direction d);

private:
    Cell& cell(Location l) {
        return data[l.y * size.width + l.x];
    }

    bool markToPosition(Location start, Location l);

};