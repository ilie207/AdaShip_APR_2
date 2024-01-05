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
  
}

void Board::printBoard(const std::vector<std::vector<char>>& board, std::pair<int, int> boardSize) const {
  // Display column letters for the board
  std::cout << "   ";
  for (int i = 0; i < boardSize.second; ++i) {
    char colLetter = static_cast<char>('A' + i);
    std::cout << colLetter << " ";
  }
  std::cout << std::endl;
  for (int i = 0; i < boardSize.first; ++i) {
    std::cout << std::setw(2) << i + 1 << " ";
    for (int j = 0; j < boardSize.second; ++j) {
      std::cout << board[i][j] << " ";
    }
    std::cout << std::endl;
  }
}


void Board::updateBoard(std::vector<std::vector<char>> &board, int row, int col, int length, bool horizontal, char symbol) {
  // Update the board with the placed ship
  if (horizontal) {
    if (col + length > board[0].size()) {
      return; // Ship placement would overflow horizontally
    }
    for (int j = col; j < col + length; ++j) {
      board[row][j] = symbol;
    }
  } else {
    if (row + length > board.size()) {
      return; // Ship placement would overflow vertically
    }
    for (int i = row; i < row + length; ++i) {
      board[i][col] = symbol;
    }
  }
}

bool Board::isValidCoordinate(int row, int col, std::pair<int, int> boardSize) {
  // Check if the coordinate is valid (within the board boundaries)
  return 
    row >= 0 && 
    col >= 0 && 
    row < boardSize.first && 
    col < boardSize.second;
}

bool Board::isValidTarget(int row, int col, const std::vector<std::vector<char>>& board, std::pair<int, int> boardSize) {
  // Check if the coordinate is on the board and the cell has not been hit before
  return isValidCoordinate(
    row, col, boardSize) &&
    board[row][col] != HIT_CELL && 
    board[row][col] != MISS_CELL;
}


bool Board::isValidPlacement(int row, int col, int length, bool horizontal, const std::vector<std::vector<char>> &board, std::pair<int, int> boardSize) {
  // Check if the ship can be placed at the given coordinates without going off the board or overlapping with an existing ship
  if (horizontal) {
    if (col + length > boardSize.second) {
      return false; // Ship would overflow board horizontally
    }
    for (int j = col; j < col + length; ++j) {
      if (board[row][j] != EMPTY_CELL) {
        return false; // Position is already occupied
      }
    }
  } else {
    if (row + length > boardSize.first) {
      return false; // Ship would overflow board vertically
    }
    for (int i = row; i < row + length; ++i) {
      if (board[i][col] != EMPTY_CELL) {
        return false; // Position is already occupied
      }
    }
  }
  return true; // Positioning is valid
}


bool Board::checkWin(const std::vector<std::vector<char>>& targetBoard, const std::vector<std::vector<char>>& referenceBoard, std::pair<int, int> boardSize) {
  // Check all cells to see if all ships have been sunk
  for (int row = 0; row < boardSize.first; ++row) {
    for (int col = 0; col < boardSize.second; ++col) {
      if (referenceBoard[row][col] != EMPTY_CELL &&
          referenceBoard[row][col] != HIT_CELL &&
          referenceBoard[row][col] != MISS_CELL &&
          targetBoard[row][col] != HIT_CELL) {
        return false;
      }
    }
  }
  return true;
}

void Board::resetBoards(std::pair<int,int> boardSize) {
  playerBoard.clear();
  playerBoard.resize(boardSize.first, std::vector<char>(boardSize.second, EMPTY_CELL));
  playerTargetBoard.clear();
  playerTargetBoard.resize(boardSize.first, std::vector<char>(boardSize.second, EMPTY_CELL));
  computerBoard.clear();
  computerBoard.resize(boardSize.first, std::vector<char>(boardSize.second, EMPTY_CELL));
  computerTargetBoard.clear();
  computerTargetBoard.resize(boardSize.first, std::vector<char>(boardSize.second, EMPTY_CELL));
}
