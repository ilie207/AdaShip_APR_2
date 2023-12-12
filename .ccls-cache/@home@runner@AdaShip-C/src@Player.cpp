#include "../headers/Player.h"
#include "../headers/Board.h"
#include "../headers/ComputerPlayer.h"
#include "../headers/ConifgLoader.h"

#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>

Player::Player() {
  // shipList = {{"Carrier", 5}, {"Battleship", 4}, {"Destroyer", 3},
  // {"Submarine", 3}, {"PatrolBoat", 2}};
}

void Player::placeShipsManually() {
  const auto &shipList = ConfigLoader::getInstance().getShipList();
  for (const auto &ship : shipList) {
    int length = ship.second;
    std::string shipName = ship.first;

    std::cout << "Placing " << shipName << " of length " << length << std::endl;

    bool validInput = false;

    while (!validInput) {
      std::cout << "Enter starting row and column (e.g., A 1): ";
      char startRow;
      int startCol;

      // Check if input is valid
      if (!(std::cin >> startRow >> startCol)) {
        std::cout << "Invalid input. Please try again." << std::endl;
        std::cin.clear();  // Clear error flags
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Clear input buffer
        continue;
      }

      // Convert row to index (e.g., A -> 0, B -> 1, etc.)
      int row = std::toupper(startRow) - 'A';
      int col = startCol - 1;


      // Ask the user for orientation
      std::cout << "Enter orientation (H for horizontal, V for vertical): ";
      char orientation;
      std::cin >> orientation;

      bool horizontal = (orientation == 'H' || orientation == 'h');

      // Validate input
      if (isValidCoordinate(col, row) && isValidPlacement(col, row, length, horizontal, playerBoard)) {
        // Valid placement, update the board
        updateBoard(playerBoard, col, row, length, horizontal, SHIP_CELL);
        validInput = true;
      } else {
        std::cout << "Invalid placement. Please try again." << std::endl;
        // Clear the input buffer in case of invalid input
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }
  }
}

void Player::placeShipsRandomly() {
  const auto &shipList = ConfigLoader::getInstance().getShipList();

  for (const auto &ship : shipList) {
    std::string shipName = ship.first;
    int length = ship.second;

    std::cout << "Placing " << shipName << " of length " << length
              << " randomly." << std::endl;

    for (;;) {
      int row = std::rand() % BOARD_SIZE;
      int col = std::rand() % BOARD_SIZE;
      // Randomly choose horizontal or vertical placement
      bool horizontal = std::rand() % 2 == 0;

      if (isValidPlacement(col, row, length, horizontal, playerBoard)) {
        // Check for overlap with existing ships on the playerBoard
        bool overlap = false;
        if (horizontal) {
          for (int j = col; j < col + length; ++j) {
            if (playerBoard[row][j] == SHIP_CELL) {
              overlap = true;
              break;
            }
          }
        } else {
          for (int i = row; i < row + length; ++i) {
            if (playerBoard[i][col] == SHIP_CELL) {
              overlap = true;
              break;
            }
          }
        }

        if (!overlap) {
          // Valid placement, update the board
          updateBoard(playerBoard, col, row, length, horizontal, SHIP_CELL);
          break;
        }
      }
    }
  }
}

void Player::updatePlayerBoard(int row, int col, bool &hit, bool &shipSunk) {
  // Check if the computer's move hits a ship on the player's board
  if (Board::playerBoard[row][col] == SHIP_CELL) {
    hit = true;
    playerTargetBoard[row][col] =
        HIT_CELL; // Mark the hit on the player's target board

    // Check if the entire ship is sunk
    shipSunk = true;
    char shipSymbol = playerBoard[col][row];
    for (int i = 0; i < BOARD_SIZE; ++i) {
      for (int j = 0; j < BOARD_SIZE; ++j) {
        if (Board::playerBoard[i][j] == shipSymbol &&
            computerTargetBoard[i][j] != HIT_CELL) {
          shipSunk = false;
          break;
        }
      }
      if (!shipSunk) {
        break;
      }
    }

    // If the ship is sunk, mark it on both player's and computer's target
    // boards
    if (shipSunk) {
      for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
          if (computerBoard[i][j] == shipSymbol) {
            playerTargetBoard[i][j] = HIT_CELL;
          }
        }
      }
    } else {
      // If the ship is not sunk, mark only on the player's target board
      playerTargetBoard[row][col] = HIT_CELL;
    }
  } else {
    hit = false;
    playerTargetBoard[row][col] =
        MISS_CELL; // Mark the miss on the player's target board
  }
  // Always mark the move on the player's target board, whether hit or miss
  playerTargetBoard[row][col] = (hit) ? HIT_CELL : MISS_CELL;
}

void Player::playerTurn() {
  bool hit = false;
  bool shipSunk = false;

  std::cout << "Player's Turn" << std::endl;

  for (;;) {
    std::cout << "Enter target row and column (e.g., A 1): ";
    char targetRow;
    int targetCol;
    std::cin >> targetRow >> targetCol;

    // Convert row to index (e.g., A -> 0, B -> 1, etc.)
    int row = static_cast<int>(targetRow) - static_cast<int>('A');
    int col = targetCol - 1;

    // Validate input
    if (isValidCoordinate(col, row) && isValidTarget(col, row)) {
      // Valid target, update the player's board
      updatePlayerBoard(col, row, hit, shipSunk);
      break;
    } else {
      std::cout << "Invalid target. Please try again." << std::endl;
      // Clear the input buffer in case of invalid input
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  // Display result of the player's turn
  if (hit) {
    std::cout << "Hit!";
    if (shipSunk) {
      std::cout << " You sunk a ship!";
    }
  } else {
    std::cout << "Miss!";
  }

  std::cout << std::endl;

  // Check if the player has won
  if (checkWin()) {
    std::cout << "Congratulations! You've sunk all enemy ships. You win!"
              << std::endl;
  }
}