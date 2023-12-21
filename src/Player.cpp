#include "../headers/Player.h"
#include "../headers/Board.h"
#include "../headers/ComputerPlayer.h"
#include "../headers/ConfigLoader.h"

#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>

Player::Player(ComputerPlayer& computerPlayer)
    : computerPlayer(computerPlayer) {
      initializeBoard(BOARD_SIZE, BOARD_SIZE);
}

void Player::placeShipsManually() {
  const auto &shipList = ConfigLoader::getInstance().getShipList();

  for (const auto &ship : shipList) {
    int length = ship.second;
    std::string shipName = ship.first;
    std::cout << "Placing " << shipName 
      << " of length " << length << std::endl;

    char choice;
    std::cout << "Do you want to place this ship manually or randomly? (M/R): ";
    std::cin >> choice;

    if (toupper(choice) == 'M') {
      bool validInput = false;
      while (!validInput) {
        std::cout << "Enter starting row and column (e.g., A1): ";
        char startRow;
        int startCol;
        if (!(std::cin >> startRow >> startCol)) {
          std::cout << "Invalid input. Please try again." << std::endl;
          std::cin.clear();
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          continue;
        }

        int col = toupper(startRow) - 'A';
        int row = startCol - 1; 

        std::cout << "Enter orientation (H for horizontal, V for vertical): ";
        char orientation;
        std::cin >> orientation;
        bool horizontal = (toupper(orientation) == 'H');

        if (Board::isValidCoordinate(row, col) && Board::isValidPlacement(row, col, length, horizontal, playerBoard)) {
          Board::updateBoard(playerBoard, row, col, length, horizontal, shipName[0]);
          validInput = true;
          Board::printBoard(playerBoard);
        } else {
          std::cout << "Invalid placement. Please try again." << std::endl;
          std::cin.clear();
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
      }
    } else if (toupper(choice) == 'R') {
      placeShipRandomly(ship.first, ship.second);
    } else {
      std::cout << "Invalid choice. Ship will be placed randomly." << std::endl;
      placeShipRandomly(ship.first, ship.second);
    }
  }
}

void Player::placeShipRandomly(const std::string& shipName, int length) {
  bool placed = false;
  while (!placed) {
    int row = std::rand() % BOARD_SIZE;
    int col = std::rand() % BOARD_SIZE;
    bool horizontal = std::rand() % 2 == 0;

    if (Board::isValidPlacement(row, col, length, horizontal, playerBoard)) {
      Board::updateBoard(playerBoard, row, col, length, horizontal, shipName[0]);
      placed = true;
      Board::printBoard(playerBoard);
    }
  }
}

void Player::placeShipsRandomly() {
  const auto &shipList = 
  ConfigLoader::getInstance().getShipList();

  std::srand(std::time(0)); 

  for (const auto &ship : shipList) {
    std::string shipName = ship.first;
    int length = ship.second;

    std::cout << "Placing " << shipName 
      << " of length " << length
      << " randomly." << std::endl;

    bool placed = false;
      int attempts = 0;
      while (!placed && attempts < 1000) { // We try a maximum of 1000 attempts per ship
        int row = std::rand() % BOARD_SIZE;
        int col = std::rand() % BOARD_SIZE;
        bool horizontal = std::rand() % 2 == 0;
        if (isValidPlacement(row, col, length, horizontal, playerBoard)) {
          updateBoard(playerBoard, row, col, length, horizontal, shipName[0]);
          placed = true;
        }
        attempts++;
      }
      if (!placed) {
        std::cerr << "Failed to place " << shipName << ". Try increasing the board size or reducing the number of ships." << std::endl;
        exit(EXIT_FAILURE); // Exit the program if we couldn't place a ship
      }
    }
  }

void Player::updatePlayerBoard(int row, int col, bool &hit, bool &shipSunk) {
  for (const auto &ship : shipList) {
  std::string shipName = ship.first;
  }
    // Check if the player's move hits a ship on the computer's board
  if (computerPlayer.computerBoard[row][col] != EMPTY_CELL) {
    hit = true;
    playerTargetBoard[row][col] = HIT_CELL; 
    shipSunk = true;
   // Check if the entire ship is sunk
    char shipSymbol = computerPlayer.computerBoard[row][col];

    // Check if all parts of the ship are hit
    for (int i = 0; i < BOARD_SIZE; ++i) {
      for (int j = 0; j < BOARD_SIZE; ++j) {
        if (computerPlayer.computerBoard[i][j] == shipSymbol &&
          playerTargetBoard[i][j] != HIT_CELL) {
              shipSunk = false; 
              break;
        }
      }
       if (!shipSunk) { 
          break;
       }
    }
    if (shipSunk) {
// Mark all the segments of the no longer existent ship as hit
      for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
          if (computerPlayer.computerBoard[i][j] == shipSymbol){
              playerTargetBoard[i][j] = HIT_CELL;
          }
        }
      }
    }
  } else {
        hit = false;
        playerTargetBoard[row][col] = MISS_CELL; // Mark the miss
    }
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
    int col = toupper(targetRow) - 'A';
    int row = targetCol - 1;

    // Validate input
    if (isValidCoordinate(row, col) && isValidTarget(row, col)){
      // Valid target, update the player's board
      updatePlayerBoard(row, col, hit, shipSunk);
      break;
    } else {
      std::cout << "Invalid target. Please try again.\n";
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
  if (checkWin(playerTargetBoard, computerPlayer.computerBoard)) {
    std::cout 
    << "Congratulations! You've sunk all enemy ships. You win!"
    << std::endl;
  }
}

void Player::printBoards() const {
  std::cout << "Player's Board:" << std::endl;
  printBoard(playerBoard);
  std::cout << std::endl;
  std::cout << "Player's Target Board:" << std::endl;
  printBoard(playerTargetBoard);
}

const std::vector<std::vector<char>>& Player::getPlayerBoard() const {
  return playerBoard;
}

bool Player::isGameOver() {
  // Verifying if all ships are sunk on either board
  if (checkWin(playerTargetBoard, computerPlayer.computerBoard)) {
    return true;
  } else if (checkWin(computerPlayer.computerTargetBoard, playerBoard)) {
    return true;
  }
  return false;
}