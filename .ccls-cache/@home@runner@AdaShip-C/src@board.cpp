#include "board.h"
#include <iostream>
#include <vector>

const int BOARD_SIZE = 5;

class BattleshipBoard {
private:
    std::vector<std::vector<char>> board;

public:
    BattleshipBoard() {
        // Initialize the board with empty sea ('~')
        board.resize(BOARD_SIZE, std::vector<char>(BOARD_SIZE, '~'));
    }

    void displayBoard() const {
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                std::cout << board[i][j] << ' ';
            }
            std::cout << std::endl;
        }
    }
};