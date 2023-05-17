/**
 * @file Tile.hpp
 * @brief Tile class header file
 * @author Parker Cranfield
*/

#ifndef TILE_HPP
#define TILE_HPP

#include <iostream>

class Tile {
    private:
        // -1 = bomb, 0 = empty, 1-8 = number of bombs adjacent
        int m_value;

        // 0 = hidden, 1 = revealed, 2 = flagged, 3 = question mark
        int m_state;

    public:
        Tile();
        ~Tile();

        void setValue(int value);
        void setState(int state);
        int getValue();
        int getState();
};

#endif // TILE_HPP