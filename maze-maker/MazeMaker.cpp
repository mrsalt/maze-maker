#include <exception>
#include <cstdlib>
#include "MazeMaker.h"

using namespace std;

int MazeMaker::addPath(Direction direction, int distance)
{
    int i = 0;
    for (; i < distance; i++) {
        Location newPosition = position + direction;
        if (!model.markCell(newPosition, direction))
            break;
        position = newPosition;
        if (position == model.endPosition)
            break;
    }
    return i;
}

void MazeMaker::unwindPath(Direction direction, int distance)
{
    // unwind what we just did
    for (int i = distance; i > 0; i--) {
        model.markCell(position, Direction::EMPTY);
        position = position - direction;
    }
}

size_t MazeMaker::countyEmptyCellsWithAdjacentOccupiedCells(int y) const
{
    size_t candidates = 0;
    for (int x = 0; x < model.size.width; x++) {
        Location l{ x, y };
        if (model.getCell(l).isEmpty() && model.nonEmptyNeighbors(l) > 0) {
            candidates++;
        }
    }
    return candidates;
}

Location MazeMaker::findBranch() const
{
    size_t nonEmptyRows = 0;
    for (size_t emptyCount : model.emptyCountByRow)
        if (emptyCount != 0) nonEmptyRows++;
    size_t rowToMark = rand() % nonEmptyRows;
    int y = 0;
    size_t count = 0;
    for (; y < model.emptyCountByRow.size(); y++) {
        if (model.emptyCountByRow[y] != 0) {
            if (count == rowToMark)
                break;
            count++;
        }
    }
    size_t candidates = countyEmptyCellsWithAdjacentOccupiedCells(y);
    while (candidates == 0) {
        y++;
        if (y == model.size.height)
            y = 0;
        candidates = countyEmptyCellsWithAdjacentOccupiedCells(y);
    }
    size_t columnToMark = rand() % candidates;
    candidates = 0;
    int x = 0;
    for (; x < model.size.width; x++) {
        Location l{ x, y };
        if (model.getCell(l).isEmpty() && model.nonEmptyNeighbors(l) > 0) {
            if (candidates == columnToMark)
                break;
            candidates++;
        }
    }
    return { x, y };
}

void MazeMaker::go()
{
    model.markCell(model.startPosition, Direction::START);

    while (position != model.endPosition) {
        Location l = position;
        Direction direction = (Direction)(1 + rand() % 4);
        int distance = 1 + rand() % 10;
        int marked = addPath(direction, distance);
        if (marked > 0) {
            if (!model.canBeCompleted(position)) {
                unwindPath(direction, marked);
                if (position != l) {
                    throw exception("Unwind did not return to original position");
                }
            }
        }
    }

    while (model.getCountEmpty() > 0) {
        Location branch = findBranch();
        Location from = model.pickNeighbor(branch, rand());
        Direction d = branch - from;
        position = from;
        if (addPath(d, 1) != 1)
            throw exception("failed to add path");
        while (model.emptyNeighbors(position) > 0) {
            Direction direction = (Direction)(1 + rand() % 4);
            int distance = 1 + rand() % 10;
            addPath(direction, distance);
        }
    }
}
