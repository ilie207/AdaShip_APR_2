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
      auto boardSize = ConfigLoader::getInstance().getBoardSize();
        initializeBoard(boardSize.first, boardSize.second);
}

void Player::placeShipsManually() {
  const auto &shipList = ConfigLoader::getInstance().getShipList();

  const auto boardSize = ConfigLoader::getInstance().getBoardSize();

  for (const auto &ship : shipList) {
    int length = ship.second;
    std::string shipName = ship.first;
    std::cout << "Placing " << shipName 
      << " of length " << length << std::endl;


    char choice;
    bool validChoice = false;
    while (!validChoice) {
      std::cout << "Do you want to place this ship manually or randomly? (M/R): ";
      std::cin >> choice;

      if (toupper(choice) == 'M') {
        validChoice = true;
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

          char orientation;
          bool horizontal;
          std::cout << "Enter orientation (H for horizontal, V for vertical): ";
          std::cin >> orientation;

          if (std::cin.fail() || (toupper(orientation) != 'H' && toupper(orientation) != 'V')) {
            std::cout << "Invalid input. Please enter 'H' for horizontal or 'V' for vertical." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
          }

          horizontal = (toupper(orientation) == 'H');

          if (Board::isValidCoordinate(row, col, boardSize) && Board::isValidPlacement(row, col, length, horizontal, playerBoard, boardSize)) {
            Board::updateBoard(playerBoard, row, col, length, horizontal, shipName[0]);
            validInput = true;
            Board::printBoard(playerBoard, boardSize);
          } else {
            std::cout << "Invalid placement. Please try again." << std::endl;
          }
        }
      } else if (toupper(choice) == 'R') {
        validChoice = true;
        placeShipRandomly(ship.first, ship.second);
      } else {
        std::cout << "Invalid choice. Please try again." << std::endl;
        // Clear the input buffer to handle the next input correctly
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }
  }
}

void Player::placeShipRandomly(const std::string& shipName, int length) {
  
  const auto boardSize = ConfigLoader::getInstance().getBoardSize();
  const int rows = boardSize.first;
  const int cols = boardSize.second;
  
  bool placed = false;
  while (!placed) {
    int row = std::rand() % rows;
    int col = std::rand() % cols;
    bool horizontal = std::rand() % 2 == 0;

    if (Board::isValidPlacement(row, col, length, horizontal, playerBoard, boardSize)) {
      Board::updateBoard(playerBoard, row, col, length, horizontal, shipName[0]);
      placed = true;
      Board::printBoard(playerBoard, boardSize);
    }
  }
}

void Player::placeShipsRandomly() {
  const auto &shipList = 
  ConfigLoader::getInstance().getShipList();

  std::srand(std::time(0)); 

  const auto boardSize = ConfigLoader::getInstance().getBoardSize();
  const int rows = boardSize.first;
  const int cols = boardSize.second;

  for (const auto &ship : shipList) {
    std::string shipName = ship.first;
    int length = ship.second;

    std::cout << "Board size (rows x cols): " << boardSize.first << " x " << boardSize.second << std::endl;
    std::cout << "Initializing player's board...\n";
    printBoards();

    std::cout << "Placing " << shipName 
      << " of length " << length
      << " randomly." << std::endl;

    bool placed = false;
      int attempts = 0;
      while (!placed && attempts < 1000) { // A maximum of 1000 attempts per ship
        int row = std::rand() % rows;
        int col = std::rand() % cols;
        bool horizontal = std::rand() % 2 == 0;
        if (isValidPlacement(row, col, length, horizontal, playerBoard, boardSize)) {
          updateBoard(playerBoard, row, col, length, horizontal, shipName[0]);
          placed = true;
        }
        attempts++;
      }
      if (!placed) {
        std::cerr << "Failed to place " << shipName << ". Try increasing the board size or reducing the number of ships." << std::endl;
         // Exit the program if we couldn't place a ship
        exit(EXIT_FAILURE);
      }
    }
  }

void Player::updatePlayerBoard(int row, int col, bool &hit, bool &shipSunk) {
  for (const auto &ship : shipList) {
  std::string shipName = ship.first;
  }

  const auto boardSize = ConfigLoader::getInstance().getBoardSize();
  const int rows = boardSize.first;
  const int cols = boardSize.second;
  
    // Check if the player's move hits a ship on the computer's board
  if (computerPlayer.computerBoard[row][col] != EMPTY_CELL) {
    hit = true;
    playerTargetBoard[row][col] = HIT_CELL; 
    shipSunk = true;
   // Check if the entire ship is sunk
    char shipSymbol = computerPlayer.computerBoard[row][col];

    // Check if all parts of the ship are hit
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      if (
        computerPlayer.computerBoard[i][j] == shipSymbol &&
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
      for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
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

  const auto boardSize = ConfigLoader::getInstance().getBoardSize();

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
    if (isValidCoordinate(row, col, boardSize) && isValidTarget(row, col, computerBoard, boardSize)){
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
  if (checkWin(playerTargetBoard, computerPlayer.computerBoard, boardSize)) {
    std::cout 
    << "Congratulations! You've sunk all enemy ships. You win!"
    << std::endl;
  }
}

void Player::printBoards() const {
  auto boardSize = ConfigLoader::getInstance().getBoardSize();
  std::cout << "Player's Board:" << std::endl;
  printBoard(playerBoard, boardSize);
  std::cout << std::endl;
  std::cout << "Player's Target Board:" << std::endl;
  printBoard(playerTargetBoard, boardSize);
}

const std::vector<std::vector<char>>& Player::getPlayerBoard() const {
  return playerBoard;
}

bool Player::isGameOver() {
  auto boardSize = ConfigLoader::getInstance().getBoardSize();
  // Verifying if all ships are sunk on either board
  if (checkWin(playerTargetBoard, computerPlayer.computerBoard, boardSize)) {
    return true;
  } else if (checkWin(computerPlayer.computerTargetBoard, playerBoard, boardSize)) {
    return true;
  }
  return false;
}