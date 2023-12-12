#include "../headers/Board.h"
#include "../headers/Player.h"
#include "../headers/ConifgLoader.h"

#include <iomanip>
#include <iostream>

void Board::initializeBoard(int rows, int cols) {

  playerBoard.resize(rows, std::vector<char>(cols, EMPTY_CELL));

  playerTargetBoard.resize(rows, std::vector<char>(cols, EMPTY_CELL));

  computerBoard.resize(rows, std::vector<char>(cols, EMPTY_CELL));

  computerTargetBoard.resize(rows, std::vector<char>(cols, EMPTY_CELL));
}

void Board::printBoards() {
  // Print player's board
  std::cout << "Player's Board:" << std::endl;

  std::cout << "   ";
  for (int i = 0; i < BOARD_SIZE; ++i) {
    char colLetter = static_cast<char>('A' + i);
    std::cout << colLetter << " ";
  }
  std::cout << std::endl;

  for (int i = 0; i < BOARD_SIZE; ++i) {
    // Display row numbers at the beginning of each row
    std::cout << std::setw(2) << i + 1 << " ";
    for (int j = 0; j < BOARD_SIZE; ++j) {
      std::cout << playerBoard[i][j] << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;

  // Print player's target board
  std::cout << "Player's Target Board:" << std::endl;

  // Display column letters for player's target board
  std::cout << "   ";
  for (int i = 0; i < BOARD_SIZE; ++i) {
    char colLetter = static_cast<char>('A' + i);
    std::cout << colLetter << " ";
  }
  std::cout << std::endl;

  for (int i = 0; i < BOARD_SIZE; ++i) {
    std::cout << std::setw(2) << i + 1 << " ";
    for (int j = 0; j < BOARD_SIZE; ++j) {
      std::cout << playerTargetBoard[i][j] << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;

  // Print computer's board
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
      std::cout << computerBoard[i][j] << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;

  // Print computer's target board
  std::cout << "Computer's Target Board:" << std::endl;

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
      std::cout << computerTargetBoard[i][j] << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
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
      if (board[row][j] == SHIP_CELL) {
        return false; // Overlaps with an existing ship
      }
    }
  } else {
    if (row + length > BOARD_SIZE) {
      return false; // Ship goes beyond the bottom edge of the board
    }

    for (int i = row; i < row + length; ++i) {
      if (board[i][col] == SHIP_CELL) {
        return false; // Overlaps with an existing ship
      }
    }
  }

  return true;
}

bool Board::checkWin() {
  const auto& shipList = ConfigLoader::getInstance().getShipList();
  // Check if all ships on both player's and computer's boards are sunk
  for (const auto &ship : shipList) {
    char shipSymbol = ship.first[0];

    // Check player's board
    for (int i = 0; i < BOARD_SIZE; ++i) {
      for (int j = 0; j < BOARD_SIZE; ++j) {
        if (playerBoard[i][j] == shipSymbol) {
          return false;
          // At least one ship is not sunk on the player's board
        }
      }
    }

    // Check computer's board
    for (int i = 0; i < BOARD_SIZE; ++i) {
      for (int j = 0; j < BOARD_SIZE; ++j) {
        if (computerBoard[i][j] == shipSymbol) {
          return false;
          // At least one ship is not sunk on the computer's board
        }
      }
    }
  }

  // All ships are sunk on both player's and computer's boards
  return true;
}

bool Board::isGameOver() {
  const auto& shipList = ConfigLoader::getInstance().getShipList();
  for (const auto &ship : shipList) {
    std::string shipName = ship.first;
    bool playerShipPresent = false;
    bool computerShipPresent = false;

    // Check if any part of the ship is still present on the player's board
    for (int i = 0; i < BOARD_SIZE; ++i) {
      for (int j = 0; j < BOARD_SIZE; ++j) {
        if (Board::playerBoard[i][j] == shipName[0] &&
            Board::playerBoard[i][j] != EMPTY_CELL) {
          playerShipPresent = true;
          break;
        }
      }
      if (playerShipPresent) {
        break;
      }
    }

    // Check if any part of the ship is still present on the computer's board
    for (int i = 0; i < BOARD_SIZE; ++i) {
      for (int j = 0; j < BOARD_SIZE; ++j) {
        if (computerBoard[i][j] == shipName[0] &&
            computerBoard[i][j] != EMPTY_CELL) {
          computerShipPresent = true;
          break;
        }
      }
      if (computerShipPresent) {
        break;
      }
    }

    // If any ship is present on either board, the game is not over
    if (playerShipPresent || computerShipPresent) {
      return false;
    }
  }

  // All ships have been sunk or not placed yet
  return true;
}