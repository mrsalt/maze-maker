#pragma once

enum class Direction
{
    EMPTY,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Location
{
public:
    size_t x;
    size_t y;

    bool operator==(const Location& rhs) const {
        return x == rhs.x && y == rhs.y;
    }

    Location operator+(Direction d) {
        switch (d) {
        case Direction::LEFT:
            return { x - 1, y };
        case Direction::RIGHT:
            return { x + 1, y };
        case Direction::UP:
            return { x, y - 1 };
        case Direction::DOWN:
            return { x, y + 1 };
        }
    }
};

class Size
{
public:
    size_t width;
    size_t height;
};



