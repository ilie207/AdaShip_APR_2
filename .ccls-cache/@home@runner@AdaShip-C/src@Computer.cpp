#include "Computer.h"
#include "Utils.h"
#include <iostream>

Computer::Computer(Board &playerBoard) : playerBoard(playerBoard), computerBoard(), computerTargetBoard() {
  computerBoard.resize(BOARD_SIZE, std::vector<char>(BOARD_SIZE, EMPTY_CELL));
  computerTargetBoard.resize(BOARD_SIZE,
                             std::vector<char>(BOARD_SIZE, EMPTY_CELL));
}

void Computer::placeComputerShipsRandomly() {
  for (const auto &ship : shipList) {
    int length = ship.second;
    std::string shipName = ship.first;

    std::cout << "Placing " << shipName << " of length " << length
              << " randomly." << std::endl;

    for (;;) {
      int row = std::rand() % BOARD_SIZE;
      int col = std::rand() % BOARD_SIZE;
      // Randomly choose horizontal or vertical placement
      bool horizontal = std::rand() % 2 == 0;

      if (isValidPlacement(row, col, length, horizontal, computerBoard)) {
        // Valid placement, update the board
        updateBoard(computerBoard, row, col, length, horizontal, SHIP_CELL);
        break;
      }
    }
  }
}

void Computer::placeComputerShipsRandomly() {
  for (const auto &ship : shipList) {
    int length = ship.second;
    std::string shipName = ship.first;

    std::cout << "Placing " << shipName << " of length " << length
              << " randomly." << std::endl;

    for (;;) {
      int row = std::rand() % BOARD_SIZE;
      int col = std::rand() % BOARD_SIZE;
      // Randomly choose horizontal or vertical placement
      bool horizontal = std::rand() % 2 == 0;

      if (isValidPlacement(row, col, length, horizontal, computerBoard)) {
        // Valid placement, update the board
        updateBoard(computerBoard, row, col, length, horizontal, SHIP_CELL);
        break;
      }
    }
  }
}

void Computer::updateComputerBoard(int row, int col, bool &hit,
                                   bool &shipSunk) {
  // Check if the computer's move hits a ship on the player's board
  if (playerBoard[row][col] == SHIP_CELL) {
    hit = true;
    computerTargetBoard[row][col] =
        HIT_CELL; // Mark the hit on the computer's target board

    // Check if the entire ship is sunk
    shipSunk = true;
    char shipSymbol = playerBoard[row][col];
    for (int i = 0; i < BOARD_SIZE; ++i) {
      for (int j = 0; j < BOARD_SIZE; ++j) {
        if (playerBoard[i][j] == shipSymbol &&
            computerTargetBoard[i][j] != HIT_CELL) {
          shipSunk = false;
          break;
        }
      }
      if (!shipSunk) {
        break;
      }
    }

    // If the ship is sunk, mark it on computer's target board
    if (shipSunk) {
      for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
          if (playerBoard[i][j] == shipSymbol) {
            computerTargetBoard[i][j] = HIT_CELL;
          }
        }
      }
    } else {
      // If the ship is not sunk, mark only on the computer's target board
      computerTargetBoard[row][col] = HIT_CELL;
    }
  } else {
    hit = false;
    computerTargetBoard[row][col] =
        MISS_CELL; // Mark the miss on the computer's target board
  }
}