/**
 * @file main.cpp
 * @brief Main file for the project
 * @author Parker Cranfield
*/

#include <iostream>
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
    Board *currentBoard;

    if (argc == 1) {
        currentBoard = new Board(10, 10, 10);
    } else if (argc == 4) {
        currentBoard = new Board(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
    } else {
        info();
        return 1;
    }

    currentBoard->info();
    currentBoard->printBoard();

    std::string input;

    do {
        std::cout << "Enter an action (h for help): ";
        std::cin >> input;

        // Help
        if (input == "h") {
            help();
        // Reveal
        } else if (input == "r") {
            int x, y;
            std::cin >> x >> y;
            currentBoard->floodAt(x, y);
            if (currentBoard->gameOver() != false && currentBoard->bombsLeft() == 0) {
                std::cout << "You win!" << std::endl;
                break;
            }
            if (currentBoard->gameOver() == true) {
                std::cout << "you lost." << std::endl;
                break;
            }
            currentBoard->printBoard();
        // Flag
        } else if (input == "f") {
            int x, y;
            std::cin >> x >> y;
            if (currentBoard->m_board[x][y].getState() != 1) currentBoard->flagAt(x, y);
            currentBoard->printBoard();
        // Question mark
        } else if (input == "q") {
            int x, y;
            std::cin >> x >> y;
            if (currentBoard->m_board[x][y].getState() != 1) currentBoard->questionAt(x, y);
            currentBoard->printBoard();
        // Unmark
        } else if (input == "u") {
            int x, y;
            std::cin >> x >> y;
            currentBoard->unmarkAt(x, y);
            currentBoard->printBoard();
        // Info
        } else if (input == "i") {
            currentBoard->info();
        } else if (input == "p") {
            int reveal;
            std::cin >> reveal;
            if (reveal == 0) currentBoard->printBoard();
            else if (reveal == 1) currentBoard->printBoard(true);
        // Bombs left
        } else if (input == "b") {
            std::cout << "Bombs left: " << currentBoard->bombsLeft() << std::endl;
        // Quit game
        } else if (input == "quit") {
            std::cout << "Quitting..." << std::endl;
        } else {
            std::cout << "Invalid action!" << std::endl;
        }

    } while (input != "quit");


    delete currentBoard;
    return 0;
}