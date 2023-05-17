/**
 * @file Board.cpp
 * @brief Board class implementation file
 * @author Parker Cranfield
*/

#include "Board.hpp"

/**
 * @brief Board constructor
*/
Board::Board(int x, int y, int numOfBombs) : m_X(x), m_Y(y), m_numOfBombs(numOfBombs) {
    // Seed random
    srand(time(0));

    // Check for errors
    if (m_X < 4 || m_Y < 4) {
        std::cout << "Board too small!" << std::endl;
        exit(1);
    }

    if (m_X > 99 || m_Y > 99) {
        std::cout << "Board too big!" << std::endl;
        exit(1);
    }

    if (numOfBombs < 1) {
        std::cout << "Not enough bombs!" << std::endl;
        exit(1);
    }
    
    if (numOfBombs > (m_X * m_Y)-10) {
        std::cout << "Too many bombs!" << std::endl;
        exit(1);
    }

    // Setup board Vector of Vectors of type Tile
    for (int i = 0; i < m_Y; i++) {
        m_board.push_back(std::vector<Tile>());
        for (int j = 0; j < m_X; j++) {
            m_board[i].push_back(Tile());
        }
    }

    // Place bombs
    placeBombs();

    // Place numbers
    placeNumbers();
}

/**
 * @brief Board destructor
*/
Board::~Board() {
    // Nothing right now
}

/**
 * @brief Print X row
*/
void Board::printX() {
    std::cout << "   |";
    for (int i = 0; i < m_X; i++) {
        if (i < 10) {
            std::cout << " " << i << " |";
            continue;
        }
        if (i > 9 && i < 100) {
            std::cout << i << " |";
            continue;
        }
    }
    std::cout << std::endl;
}

/**
 * @brief Print a line
*/
void Board::printLine() {
    for (int i = 0; i <= m_X; i++) {
        std::cout << "---+";
    }
    std::cout << "---" << std::endl;
}

/**
 * @brief Check if the given x y is in the board
*/
bool Board::inBoard(int x, int y) {
    return (x >= 0 && y >= 0 && x < m_X && y < m_Y);
}

/**
 * @brief Place bombs
*/
void Board::placeBombs() {
    int x;
    int y;
    for (int i = 0; i < m_numOfBombs; i++) {
        do {
            x = rand() % m_X;
            y = rand() % m_Y;
        } while (m_board[x][y].getValue() == -1);
        m_board[x][y].setValue(-1);
    }
}

/**
 * @brief Increment adjacent tiles
*/
void Board::boardIncAdj(int x, int y) {
    for (int yCheck = -1; yCheck <= 1; yCheck++) {
        for (int xCheck = -1; xCheck <= 1; xCheck++) {
            int newX = x + xCheck;
            int newY = y + yCheck;
            if (!inBoard(newX, newY) || m_board[newX][newY].getValue() == -1) continue;
            m_board[newX][newY].setValue(m_board[newX][newY].getValue() + 1);
        }
    }

}

/**
 * @brief Place numbers
*/
void Board::placeNumbers() {
    for (int y = 0; y < m_Y; y++) {
        for (int x = 0; x < m_X; x++) {
            if (m_board[x][y].getValue() == -1) boardIncAdj(x, y);
        }
    }
}

/**
 * @brief Get the number of tiles left
*/
int Board::boardTilesLeft() {
    int tiles = 0;
    for (int y = 0; y < m_Y; y++) {
        for (int x = 0; x < m_X; x++) {
            if (m_board[x][y].getState() == 0) tiles++;
        }
    }
    return tiles;
}

/**
 * @brief Info about the board
*/
void Board::info() {
    std::cout << "Board info" << std::endl;
    std::cout << "\tX: " << m_X << std::endl;
    std::cout << "\tY: " << m_Y << std::endl;
    std::cout << "\tNumber of bombs: " << m_numOfBombs << std::endl;
    std::cout << std::endl;
}

/**
 * @brief Print the board
*/
void Board::printBoard(bool reveal) {
    printX();
    printLine();
    for (int y = 0; y < m_Y; y++) {
        // Spacing
        if (y < 10) std::cout << y << "  |";
        else if (y > 9 && y < 100) std::cout << y << " |";

        for (int x = 0; x < m_X; x++) {
            switch (m_board[x][y].getState()) {
                // hidden
                case 0:
                    if (reveal) {
                        if (m_board[x][y].getValue() == -1) {
                            std::cout << "[B]|";
                        } else {
                            if (m_board[x][y].getValue() == 0) {
                                std::cout << "   |";
                            } else {
                                std::cout << " " << m_board[x][y].getValue() << " |";
                            }
                        }
                    } else {
                        std::cout << "[ ]|";
                    }
                    break;
                
                // revealed
                case 1:
                    if (m_board[x][y].getValue() == -1) {
                        std::cout << "}#{|";
                    } else {
                        if (m_board[x][y].getValue() == 0) {
                            std::cout << "   |";
                        } else {
                            std::cout << " " << m_board[x][y].getValue() << " |";
                        }
                    }
                    break;

                // flagged
                case 2:
                    std::cout << "[F]|";
                    break;
                
                // question mark
                case 3:
                    std::cout << "[?]|";
                    break;
                
                default:
                    std::cout << "Tile at (" << x << ", " << y << ") has an invalid state!";
                    break;
            }
        }
        std::cout << y << std::endl;
        printLine();
    }
    printX();
}

/**
 * @brief Flood at a certain point
*/
void Board::floodAt(int x, int y) {
    m_board[x][y].setState(1);
    
    if (m_board[x][y].getValue() == -1) {
        m_gameOver = true;
        return;
    }

    for (int yCheck = -1; yCheck <= 1; yCheck++) {
        for (int xCheck = -1; xCheck <= 1; xCheck++) {
            int newX = x+xCheck;
            int newY = y+yCheck;
            if ((yCheck == 0 && xCheck == 0) || !inBoard(newX, newY)) continue;
            if (m_board[newX][newY].getValue() == 0 && m_board[newX][newY].getState() == 0) floodAt(newX, newY);
            else if (m_board[newX][newY].getValue() > 0 && m_board[newX][newY].getState() == 0) m_board[newX][newY].setState(1);
        }
    }
}

/**
 * @brief Flag at a certain point
*/
void Board::flagAt(int x, int y) {
    if (m_board[x][y].getState() == 0) m_board[x][y].setState(2);
    else std::cout << "Tile at (" << x << ", " << y << ") is already revealed!" << std::endl;
    if (bombsLeft() == 0) m_gameOver = true;
}

/**
 * @brief Question mark at a certain point
*/
void Board::questionAt(int x, int y) {
    if (m_board[x][y].getState() == 0) m_board[x][y].setState(3);
    else std::cout << "Tile at (" << x << ", " << y << ") is already revealed!" << std::endl;
}

/**
 * @brief Unmark at a certain point
*/
void Board::unmarkAt(int x, int y) {
    if (m_board[x][y].getState() == 2 || m_board[x][y].getState() == 3) m_board[x][y].setState(0);
    else std::cout << "Tile at (" << x << ", " << y << ") is already revealed!" << std::endl;
}

/**
 * @brief Get the number of bombs left
*/
int Board::bombsLeft() {
    int bombs = 0;
    for (int y = 0; y < m_Y; y++) {
        for (int x = 0; x < m_X; x++) {
            if (m_board[x][y].getValue() == -1 && m_board[x][y].getState() != 2) bombs++;
        }
    }
    return bombs;
}

/**
 * @brief Check if the game is over
*/
bool Board::gameOver() {
    return m_gameOver;
}