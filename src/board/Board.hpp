/**
 * @file Board.hpp
 * @brief Board class header file
 * @author Parker Cranfield
*/

#ifndef BOARD_HPP
#define BOARD_HPP

#include <iostream>
#include <vector>
#include "Tile.hpp"

class Board {
    private:
        /* data */
        const int m_X;
        const int m_Y;
        const int m_numOfBombs;
        bool m_gameOver = false;

        void printX();
        void printLine();

        bool inBoard(int x, int y);
        void placeBombs();
        void placeNumbers();
        void boardIncAdj(int x, int y);
        int boardTilesLeft();
    public:
        Board(int x, int y, int numOfBombs);
        ~Board();
        std::vector<std::vector<Tile>> m_board;

        void info();
        void printBoard(bool reveal = false);
        void floodAt(int x, int y);
        void flagAt(int x, int y);
        void questionAt(int x, int y);
        void unmarkAt(int x, int y);
        int bombsLeft();
        bool gameOver();

};

#endif // BOARD_HPP