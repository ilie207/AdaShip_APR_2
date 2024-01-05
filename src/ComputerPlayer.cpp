#include "../headers/ComputerPlayer.h"
#include "../headers/ConfigLoader.h"
#include "../headers/Board.h"
#include "../headers/Player.h"

#include <chrono>
#include <iostream>
#include <thread>

ComputerPlayer::ComputerPlayer() {
auto boardSize = ConfigLoader::getInstance().getBoardSize();
initializeBoard(boardSize.first, boardSize.second);
}

void ComputerPlayer::placeComputerShipsRandomly() {
  const auto& shipList = ConfigLoader::getInstance().getShipList();

  const auto boardSize = ConfigLoader::getInstance().getBoardSize();
  const int rows = boardSize.first;
  const int cols = boardSize.second;

  for (const auto &ship : shipList) {
    int length = ship.second;
    std::string shipName = ship.first;

    std::cout << "Placing " << shipName << " of length " << length
              << " randomly." << std::endl;

    while (true) {
      int row = std::rand() % rows;
      int col = std::rand() % cols;
      // Randomly choose horizontal or vertical placement
      bool horizontal = std::rand() % 2 == 0;

      if (Board::isValidPlacement(row, col, length, horizontal, computerBoard, boardSize)) {
        Board::updateBoard(computerBoard, row, col, length, horizontal, shipName[0]);
        break;
      }
    }
  }
}

void ComputerPlayer::computerTurn() {
  bool hit = false;
  bool shipSunk = false;

  const auto boardSize = ConfigLoader::getInstance().getBoardSize();
  const int rows = boardSize.first;
  const int cols = boardSize.second;

  std::cout << "Computer's Turn" << std::endl;

  std::this_thread::sleep_for(std::chrono::seconds(1));
  // Loop until a valid target is selected
  while (true) {
    // Randomly select a target cell
    int row = std::rand() % rows;
    int col = std::rand() % cols;
    // Check if the target cell is already hit or missed
    if (isValidCoordinate(row, col, boardSize) && 
      isValidTarget(row, col, player->getPlayerBoard(), boardSize)) {
      // Announce the computer's target
      std::cout << "Computer fires at " << static_cast<char>('A' + row) << col + 1 << std::endl;

      // Valid target, update the computer's target board
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
}


void ComputerPlayer::updateComputerBoard(int row, int col, bool &hit, bool &shipSunk) {

  const auto boardSize = ConfigLoader::getInstance().getBoardSize();
  const int rows = boardSize.first;
  const int cols = boardSize.second;
  
// Check if the player's move hits a ship on the computer's board
  if (player->getPlayerBoard()[row][col] != EMPTY_CELL) {
    hit = true;
    computerTargetBoard[row][col] = HIT_CELL;

    // Check if the entire ship is sunk
    shipSunk = true;
    char shipSymbol = player->getPlayerBoard()[row][col];
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        if (player->getPlayerBoard()[i][j] == shipSymbol &&
            computerTargetBoard[i][j] != HIT_CELL) {
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
      for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
          if (player->getPlayerBoard()[i][j] == shipSymbol) {
            computerTargetBoard[i][j] = SHIP_SUNK;
          }
        }
      }
    }
  } else {
    hit = false;
    computerTargetBoard[row][col] =
        MISS_CELL; // Mark the miss on the computer's target board
  }
}


void ComputerPlayer::printBoards() const {
  auto boardSize = ConfigLoader::getInstance().getBoardSize();
  std::cout << "Computer's Board:" << std::endl;
  printBoard(computerBoard, boardSize);
  std::cout << std::endl;
  std::cout << "Computer's Target Board:" << std::endl;
  printBoard(computerTargetBoard, boardSize);
}


void ComputerPlayer::setPlayer(Player* playerObj) {
    player = playerObj;
}