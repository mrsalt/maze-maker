#include "MazeModel.h"

using namespace std;

MazeModel::MazeModel(Size size, Location start, Location end)
    : size(size)
    , startPosition(start)
    , endPosition(end)
{
    data.resize(size.width * size.height, Cell(Direction::EMPTY));
    countEmpty = data.size();
    for (int r = 0; r < size.height; r++) {
        emptyCountByRow.push_back(size.width);
    }
}

MazeModel::MazeModel(const MazeModel& src)
    : size(src.size)
    , startPosition(src.startPosition)
    , endPosition(src.endPosition)
    , data(src.data)
    , emptyCountByRow(src.emptyCountByRow)
{
    countEmpty = src.countEmpty;
}

size_t MazeModel::nonEmptyNeighbors(Location l) const
{
    size_t occupied = 0;
    Location neighbors[4] = { l + Direction::LEFT, l + Direction::RIGHT, l + Direction::UP, l + Direction::DOWN };
    for (Location n : neighbors) {
        if (inBounds(n) && !getCell(n).isEmpty())
            occupied++;
    }
    return occupied;
}

size_t MazeModel::emptyNeighbors(Location l) const
{
    size_t empty = 0;
    Location neighbors[4] = { l + Direction::LEFT, l + Direction::RIGHT, l + Direction::UP, l + Direction::DOWN };
    for (Location n : neighbors) {
        if (inBounds(n) && getCell(n).isEmpty())
            empty++;
    }
    return empty;
}

Location MazeModel::pickNeighbor(Location l, int rand_value) const
{
    size_t occupied = nonEmptyNeighbors(l);
    Location neighbors[4] = { l + Direction::LEFT, l + Direction::RIGHT, l + Direction::UP, l + Direction::DOWN };
    int nth = rand_value % occupied;
    occupied = 0;
    for (Location n : neighbors) {
        if (inBounds(n) && !getCell(n).isEmpty()) {
            if (occupied == nth)
                return n;
            occupied++;
        }
    }
    throw exception("program bug");
}

bool MazeModel::markCell(Location l, Direction d)
{
    if (!inBounds(l))
        return false;
    Cell& cellToMark = cell(l);

    if ((d == Direction::EMPTY) == cellToMark.isEmpty())
        return false;

    cellToMark.fromDirection = d;
    
    int changeInEmpty = d == Direction::EMPTY ? 1 : -1;
    emptyCountByRow[l.y] += changeInEmpty;
    countEmpty += changeInEmpty;
    return true;
}

bool MazeModel::canBeCompleted(Location l) const
{
    MazeModel copy(*this);
    return copy.recursiveMarkToPosition(endPosition, l);
}

bool MazeModel::recursiveMarkToPosition(Location start, Location end)
{
    if (!inBounds(start))
        return false;

    if (start == end)
        return true;

    Cell& current = cell(start);
    if (!current.isEmpty())
        return false;

    cell(start).fromDirection = Direction::MARKED;

    return recursiveMarkToPosition(start + Direction::LEFT, end) ||
        recursiveMarkToPosition(start + Direction::RIGHT, end) ||
        recursiveMarkToPosition(start + Direction::UP, end) ||
        recursiveMarkToPosition(start + Direction::DOWN, end);
}
