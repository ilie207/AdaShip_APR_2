#include "../headers/Board.h"
#include "../headers/Player.h"
#include "../headers/ConfigLoader.h"

#include <iomanip>
#include <iostream>

void Board::initializeBoard(int rows, int cols) {

  playerBoard.resize(rows, std::vector<char>(cols, EMPTY_CELL));

  playerTargetBoard.resize(rows, std::vector<char>(cols, EMPTY_CELL));

  computerBoard.resize(rows, std::vector<char>(cols, EMPTY_CELL));

  computerTargetBoard.resize(rows, std::vector<char>(cols, EMPTY_CELL));
}

char getShipSymbolFromName(const std::string &name) {
  return name[0];
}

void Board::printBoards() const {
/*  // Print player's board
  std::cout << "Player's Board:" << std::endl;
  printBoard(playerBoard);

  std::cout << std::endl;

  // Print player's target board
  std::cout << "Player's Target Board:" << std::endl;
  printBoard(playerTargetBoard);

  std::cout << std::endl;

  // Print computer's target board
  std::cout << "Computer's Target Board:" << std::endl;
  printBoard(computerTargetBoard);

  std::cout << std::endl;

  // Print computer's board without revealing the ships (not shown to the player)
  std::cout << "Computer's Board:" << std::endl;

  // Display column letters for computer's board
  std::cout << "   ";
  for (int i = 0; i < BOARD_SIZE; ++i) {
    char colLetter = static_cast<char>('A' + i);
    std::cout << colLetter << " ";
  }
  std::cout << std::endl;

  for (int i = 0; i < BOARD_SIZE; ++i) {
    std::cout << std::setw(2) << i + 1 << " ";
    for (int j = 0; j < BOARD_SIZE; ++j) {
      char cellValue = computerBoard[i][j];
      if (cellValue == SHIP_CELL) {  // Hide the actual SHIP_CELLs when displaying the computer's board
        std::cout << EMPTY_CELL << " ";
      } else {
        std::cout << cellValue << " ";  // Display hits and misses
      }
    }
    std::cout << std::endl;
  }
  std::cout << std::endl; */
}

void Board::printBoard(const std::vector<std::vector<char>>& board) const {
  // Display column letters for the board
  std::cout << "   ";
  for (int i = 0; i < BOARD_SIZE; ++i) {
    char colLetter = static_cast<char>('A' + i);
    std::cout << colLetter << " ";
  }
  std::cout << std::endl;

  for (int i = 0; i < BOARD_SIZE; ++i) {
    std::cout << std::setw(2) << i + 1 << " ";
    for (int j = 0; j < BOARD_SIZE; ++j) {
      std::cout << board[i][j] << " ";
    }
    std::cout << std::endl;
  }
}


void Board::updateBoard(std::vector<std::vector<char>> &board, int row, int col, int length, bool horizontal, char symbol) {
  // Update the board with the placed ship
  if (horizontal) {
    for (int j = col; j < col + length; ++j) {
      board[row][j] = symbol;
    }
  } else {
    for (int i = row; i < row + length; ++i) {
      board[i][col] = symbol;
    }
  }
}

bool Board::isValidCoordinate(int row, int col) {
  // Check if the coordinate is valid (within the board)
  return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}

bool Board::isValidTarget(int row, int col) {
  // Check if the coordinate is valid (within the board) and the target has not
  // been hit or sunk before
  return isValidCoordinate(row, col) &&
         playerTargetBoard[row][col] != HIT_CELL &&
         playerTargetBoard[row][col] != MISS_CELL;
}


bool Board::isValidPlacement(int row, int col, int length, bool horizontal, const std::vector<std::vector<char>> &board) {
  // Check if the placement is valid
  if (horizontal) {
    if (col + length > BOARD_SIZE) {
      return false; // Ship goes beyond the right edge of the board
    }

    for (int j = col; j < col + length; ++j) {
      if (board[row][j] != EMPTY_CELL) {
        return false; // Overlaps with an existing ship
      }
    }
  } else {
    if (row + length > BOARD_SIZE) {
      return false; // Ship goes beyond the bottom edge of the board
    }

    for (int i = row; i < row + length; ++i) {
      if (board[i][col] != EMPTY_CELL) {
        return false; // Overlaps with an existing ship
      }
    }
  }

  return true;
}


bool Board::checkWin(const std::vector<std::vector<char>>& targetBoard, const std::vector<std::vector<char>>& referenceBoard) {
  // Iterate over the reference board to check if all ships have been sunk
  for (int row = 0; row < referenceBoard.size(); ++row) {
    for (int col = 0; col < referenceBoard[row].size(); ++col) {
      char referenceCell = referenceBoard[row][col];
      char targetCell = targetBoard[row][col];

      // If there's a ship part on the reference board that hasn't been hit on the target board, return false
      if (referenceCell != EMPTY_CELL &&
          referenceCell != HIT_CELL &&
          referenceCell != MISS_CELL &&
          targetCell != HIT_CELL) {
        return false;
      }
    }
  }
  // If all ship parts on the reference board have been hit on the target board, return true
  return true;
}
