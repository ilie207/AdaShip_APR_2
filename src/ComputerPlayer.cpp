#include "../headers/ComputerPlayer.h"
#include "../headers/Board.h"
#include "../headers/ConifgLoader.h"
#include "../headers/Player.h"

#include <chrono>
#include <iostream>
#include <thread>

// Board board;

void ComputerPlayer::placeComputerShipsRandomly() {

  const auto &shipList = ConfigLoader::getInstance().getShipList();

  for (const auto &ship : shipList) {
    int length = ship.second;
    std::string shipName = ship.first;

    std::cout << "Placing " << shipName << " of length " << length
              << " randomly." << std::endl;

    for (;;) {
      int row = std::rand() % BOARD_SIZE;
      int col = std::rand() % BOARD_SIZE;
      bool horizontal = std::rand() % 2 ==
                        0; // Randomly choose horizontal or vertical placement

      if (isValidPlacement(col, row, length, horizontal, computerBoard)) {
        // Check for overlap with existing ships on the computerBoard
        bool overlap = false;
        if (horizontal) {
          for (int j = col; j < col + length; ++j) {
            if (computerBoard[row][j] == SHIP_CELL) {
              overlap = true;
              break;
            }
          }
        } else {
          for (int i = row; i < row + length; ++i) {
            if (computerBoard[i][col] == SHIP_CELL) {
              overlap = true;
              break;
            }
          }
        }

        if (!overlap) {
          // Valid placement, update the board
          updateBoard(computerBoard, col, row, length, horizontal, SHIP_CELL);
          break;
        }
      }
    }
  }
}

void ComputerPlayer::computerTurn() {
  bool hit = false;
  bool shipSunk = false;

  std::cout << "Computer's Turn" << std::endl;

  // Implementing a simple auto-fire option for the computer
  for (int attempts = 0; attempts < 5; ++attempts) {
    int row = std::rand() % BOARD_SIZE;
    int col = std::rand() % BOARD_SIZE;

    if (isValidCoordinate(col, row) &&
        computerTargetBoard[row][col] == EMPTY_CELL) {
      std::cout << "Computer fires at " << static_cast<char>(row + 'A')
                << col + 1 << std::endl;
      std::this_thread::sleep_for(
          std::chrono::seconds(2)); // Simulate some delay for dramatic effect

      // Valid target, update the computer's board
      updateComputerBoard(col, row, hit, shipSunk);
      break;
    }
  }

  // Display result of the computer's turn
  if (hit) {
    std::cout << "Hit!";
    if (shipSunk) {
      std::cout << " Computer sunk a ship!";
    }
  } else {
    std::cout << "Miss!";
  }

  std::cout << std::endl;

  // Check if the computer has won
  if (checkWin()) {
    std::cout << "Computer has sunk all your ships. You lose!" << std::endl;
    // Add additional logic if needed
  }

  // Require user interaction to end the computer's turn
  std::cout << "Press Enter to continue...";
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void ComputerPlayer::updateComputerBoard(int row, int col, bool &hit,
                                         bool &shipSunk) {
  // Check if the player's move hits a ship on the computer's board
  if (computerBoard[col][row] == SHIP_CELL) {
    hit = true;
    playerTargetBoard[col][row] =
        HIT_CELL; // Mark the hit on the computer's target board

    // Check if the entire ship is sunk
    shipSunk = true;
    char shipSymbol = computerBoard[row][col];
    for (int i = 0; i < BOARD_SIZE; ++i) {
      for (int j = 0; j < BOARD_SIZE; ++j) {
        if (computerBoard[i][j] == shipSymbol &&
            playerTargetBoard[i][j] != HIT_CELL) {
          shipSunk = false;
          break;
        }
      }
      if (!shipSunk) {
        break;
      }
    }

    // If the ship is sunk, mark it on the computer's target board
    if (shipSunk) {
      for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
          if (playerBoard[i][j] == shipSymbol) {
            computerTargetBoard[i][j] = HIT_CELL;
          }
        }
      }
    }
  } else {
    hit = false;
    computerTargetBoard[row][col] =
        MISS_CELL; // Mark the miss on the computer's target board
  }
  // Always mark the move on the computer's target board, whether hit or miss
  computerTargetBoard[row][col] = (hit) ? HIT_CELL : MISS_CELL;
}