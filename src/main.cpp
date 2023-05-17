/**
 * @file main.cpp
 * @brief Main file for the project
 * @author Parker Cranfield
*/

#include <iostream>
#include <memory>
#include "board/Board.hpp"

void info() {
    std::cout << "Usage: ./Minesweeper <grid width> <grid height> <num bombs>\n";
}

void help() {
    std::cout << "Action help" << std::endl;
    std::cout << "\tTo reveal a tile:\n\t\tr <x position> <y positon>" << std::endl;
    std::cout << "\tTo flag a tile:\n\t\tf <x position> <y positon>\n" << std::endl;
    std::cout << "\tTo question mark a tile:\n\t\tq <x position> <y positon>\n" << std::endl;
    std::cout << "\tTo unmark a tile:\n\t\tu <x position> <y positon>\n" << std::endl;
}

int main(int argc, char const *argv[]) {
    std::unique_ptr<Board> currentBoard(nullptr);

    if (argc == 1) {
        currentBoard = std::make_unique<Board> (10, 10, 10);
    } else if (argc == 4) {
        currentBoard = std::make_unique<Board> (atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
    } else {
        info();
        return 1;
    }

    currentBoard->info();
    currentBoard->printBoard();

    char input;
    int x, y;

    do {

        std::cout << "Enter an action (h for help): ";
        std::cin >> input;

        switch (input) {
        case 'h': // Help command
            help();
            break;
        
        case 'r': // Reveal tile at (x,y)
            std::cin >> x >> y;
            currentBoard->floodAt(x, y);
            currentBoard->printBoard();
            if (currentBoard->gameOver() == true) {
                std::cout << "you lost." << std::endl;
                break;
            }
            break;
        
        case 'f': // Flag tile at (x,y)
            std::cin >> x >> y;
            if (currentBoard->m_board[x][y].getState() != 1) currentBoard->flagAt(x, y);
            currentBoard->printBoard();
            if (currentBoard->gameOver() != false && currentBoard->bombsLeft() == 0) {
                std::cout << "You win!" << std::endl;
                break;
            }
            break;

        case 'q': // Question mark tile at (x,y)
            std::cin >> x >> y;
            if (currentBoard->m_board[x][y].getState() != 1) currentBoard->questionAt(x, y);
            currentBoard->printBoard();
            break;

        case 'u': // Unmark tile at (x,y)
            std::cin >> x >> y;
            currentBoard->unmarkAt(x, y);
            currentBoard->printBoard();
            break;

        case 'i': // Info
            currentBoard->info();
            break;

        case 'p': // Print board - it will reveal the answers depending on the input
            int reveal;
            std::cin >> reveal;
            if (reveal == 0) currentBoard->printBoard();
            else if (reveal == 1) currentBoard->printBoard(true);
            break;

        case 'b': // Bombs left
            std::cout << "Bombs left: " << currentBoard->bombsLeft() << std::endl;
            break;

        case 'e':
                std::cout << "Quitting..." << std::endl;
                return 0;
        
        default:
            std::cout << "Invalid action!" << std::endl;
            break;
        }

    } while (currentBoard->gameOver() == false);

    return 0;
}