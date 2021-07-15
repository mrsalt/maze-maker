#include <exception>
#include "maze-maker/Primitives.h"

using namespace std;

Location Location::operator+(Direction d) const {
    switch (d) {
    case Direction::LEFT:
        return { x - 1, y };
    case Direction::RIGHT:
        return { x + 1, y };
    case Direction::UP:
        return { x, y - 1 };
    case Direction::DOWN:
        return { x, y + 1 };
    default:
        throw exception("Non-standard direction");
    }
}

Location Location::operator-(Direction d) const {
    switch (d) {
    case Direction::LEFT:
        return { x + 1, y };
    case Direction::RIGHT:
        return { x - 1, y };
    case Direction::UP:
        return { x, y + 1 };
    case Direction::DOWN:
        return { x, y - 1 };
    default:
        throw exception("Non-standard direction");
    }
}

Direction Location::operator-(const Location& l) const {
    Location d{ x - l.x, y - l.y };
    if (d == Location{1, 0})
        return Direction::RIGHT;
    else if (d == Location{-1, 0})
        return Direction::LEFT;
    else if (d == Location{0, 1})
        return Direction::DOWN;
    else if (d == Location{0, -1})
        return Direction::UP;
    throw exception("Locations are not immediate neighbors");
}
