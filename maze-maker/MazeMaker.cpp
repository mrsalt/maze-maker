#include "MazeMaker.h"

bool MazeMaker::addPath(Direction direction, int distance) {
    for (int i = 0; i < distance; i++) {
        Location newPosition = position + direction;
        if (model.inBounds(newPosition)) {
            Cell& cell = model.cell(newPosition);
            if (!cell.isEmpty())
                break;
            cell.fromDirection = direction;
            position = newPosition;
        }
        else {
            break;
        }
    }
    bool canBeCompleted = model.canBeCompleted(position);
    if (!canBeCompleted) {
        // unwind what we just did
    }
    return canBeCompleted;
}
