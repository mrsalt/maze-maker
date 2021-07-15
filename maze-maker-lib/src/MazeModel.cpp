#include <stack>
#include "maze-maker/MazeModel.h"

using namespace std;

MazeModel::MazeModel(Size size, Location start, Location end, ProgressListener* listener)
    : size(size)
    , startPosition(start)
    , endPosition(end)
    , listener(listener)
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
    if (listener)
        listener->onCellChanged(l);
    return true;
}

bool MazeModel::canBeCompleted(Location l) const
{
    MazeModel copy(*this);
    return copy.markToPosition(endPosition, l);
}

bool MazeModel::markToPosition(Location start, Location end)
{
    stack<Location> toVisit;
    toVisit.push(start);

    while (!toVisit.empty()) {
        Location l = toVisit.top();
        toVisit.pop();

        if (!inBounds(l))
            continue;

        if (l == end)
            return true;

        Cell& current = cell(l);
        if (!current.isEmpty())
            continue;

        cell(l).fromDirection = Direction::MARKED;
        toVisit.push(l + Direction::LEFT);
        toVisit.push(l + Direction::RIGHT);
        toVisit.push(l + Direction::UP);
        toVisit.push(l + Direction::DOWN);
    }
    return false;
}
