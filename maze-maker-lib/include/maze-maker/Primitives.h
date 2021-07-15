#pragma once

enum class Direction
{
    EMPTY,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    START,
    MARKED
};

class Location
{
public:
    int x;
    int y;

    bool operator==(const Location& rhs) const {
        return x == rhs.x && y == rhs.y;
    }

    bool operator!=(const Location& rhs) const {
        return x != rhs.x || y != rhs.y;
    }

    Location operator+(Direction d) const;
    Location operator-(Direction d) const;
    Direction operator-(const Location & l) const;
};

class Size
{
public:
    size_t width;
    size_t height;
};



