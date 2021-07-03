#include "MazeModel.h"

using namespace std;

MazeModel::MazeModel(const MazeModel& src)
    : size(src.size)
    , startPosition(src.startPosition)
    , endPosition(src.endPosition)
    , data(src.data)
{
    countEmpty = src.countEmpty;
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

    cell(start).fromDirection = Direction::LEFT;

    return recursiveMarkToPosition(start + Direction::LEFT, end) ||
        recursiveMarkToPosition(start + Direction::RIGHT, end) ||
        recursiveMarkToPosition(start + Direction::UP, end) ||
        recursiveMarkToPosition(start + Direction::DOWN, end);
}
