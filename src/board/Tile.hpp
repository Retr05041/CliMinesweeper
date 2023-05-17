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
        // How many bombs are adjacent to this tile
        int m_value;

        // 0 = hidden, 1 = revealed, 2 = flagged, 3 = question mark
        int m_state;

        bool m_isBomb;
    public:
        Tile();
        ~Tile();

        void setBomb(bool bomb);
        void setValue(int value);
        void setState(int state);
        bool getIsBomb();
        int getValue();
        int getState();
};

#endif // TILE_HPP