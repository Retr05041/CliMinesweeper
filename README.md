# CliMinesweeper
Command line Minesweeper in C++

Rewrite of https://github.com/DogeSSBM/CliMinesweeper in C++.

### How to run
Make sure to have g++ installed (check `CMakeLists.txt` for specific requirements). Then run the following commands:
```
mkdir build
cd build
cmake ..
make
./Minesweeper
```
Optional, you can run `./MineSweeper <rows> <cols> <bombs>` to specify the size of the board and the number of bombs. Default is 10x10 with 10 bombs.

### How to play
```
h - Help

r <row> <col> - Reveal a cell
f <row> <col> - Flag a cell
q <row> <col> - Question mark a cell
u <row> <col> - Unflag a cell

i - Show information about the game
p <reveal> - Print the board (reveal = 1 to reveal all cells, 0 to show board as is) 
b - How many Bombs left
e - Quit the game
```
