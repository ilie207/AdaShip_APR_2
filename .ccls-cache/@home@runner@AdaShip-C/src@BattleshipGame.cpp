#include "BattleshipGame.h"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>
#include <limits>

const char EMPTY = '~';
const char SHIP = 'O';
const char HIT = 'X';
const char MISS = '-';

class BattleshipGame {
private:
std::vector<std::vector<char>> playerShipBoard;
std::vector<std::vector<char>> playerTargetBoard;
std::vector<std::vector<char>> computerShipBoard;
std::vector<std::vector<char>> computerTargetBoard;
bool playerTurn;
bool gameOver;
std::vector<std::string> shipNames;

public:
  BattleshipGame();
    void loadConfig(const std::string& configFileName);
    void displayBoards() const;
    void setupGame();
    void playTurn();
    bool isGameOver() const;
    bool isValidCoordinate(int row, int col, bool isComputer) const;
    void autoPlaceShips(std::vector<std::vector<char>>& board);
};

BattleshipGame::BattleshipGame() : playerTurn(true), gameOver(false) {
  std::srand(time(0));
  // Initialise ship names
  shipNames = {"Carrier", "Battleship", "Destroyer", "Submarine",
               "Patrol Boat"};

  // Initialise the board with empty sea ('~')
  playerShipBoard.resize(10, std::vector<char>(10, '~'));
  playerTargetBoard.resize(10, std::vector<char>(10, '~'));
  computerShipBoard.resize(10, std::vector<char>(10, '~'));
  computerTargetBoard.resize(10, std::vector<char>(10, '~'));
}

void BattleshipGame::loadConfig(const std::string &configFileName) {
  std::ifstream configFile(configFileName);
  if (!configFile.is_open()) {
    std::cerr << "Error opening config file: " << configFileName << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string line;
  while (std::getline(configFile, line)) {
    if (line.find("Boat: ") != std::string::npos) {
      // Extract ship name and size from the line
      size_t colonPos = line.find(":");
      size_t commaPos = line.find(",");
      if (colonPos != std::string::npos && commaPos != std::string::npos) {
        std::string shipName =
            line.substr(colonPos + 2, commaPos - colonPos - 2);
        int shipSize = std::stoi(line.substr(commaPos + 2));

        std::cout << "Ship: " << shipName << ", Size: " << shipSize
                  << std::endl;
      }
    } else if (line.find("Board: ") != std::string::npos) {
      // Extract rows and columns from the line
      size_t colonPos = line.find(":");
      size_t xPosition = line.find("x");
      if (colonPos != std::string::npos && xPosition != std::string::npos) {
        int rows =
            std::stoi(line.substr(colonPos + 2, xPosition - colonPos - 2));
        int cols = std::stoi(line.substr(xPosition + 1));

        std::cout << "Board: Rows = " << rows << ", Cols = " << cols
                  << std::endl;
      }
    }
  }
  configFile.close();
}

void BattleshipGame::displayBoards() const {
  std::cout << "Player Ship Board:" << std::endl;
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 10; ++j) {
      std::cout << playerShipBoard[i][j] << ' ';
    }
    std::cout << std::endl;
  }

  std::cout << "\nPlayer Target Board:" << std::endl;
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 10; ++j) {
      std::cout << playerTargetBoard[i][j] << ' ';
    }
    std::cout << std::endl;
  }

  std::cout << "\nComputer Ship Board:" << std::endl;
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 10; ++j) {
      std::cout << computerShipBoard[i][j] << ' ';
    }
    std::cout << std::endl;
  }

  std::cout << "\nComputer Target Board:" << std::endl;
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 10; ++j) {
      std::cout << computerTargetBoard[i][j] << ' ';
    }
    std::cout << std::endl;
  }
}

void BattleshipGame::playTurn() {
  if (playerTurn) {
    // Player's turn
    std::cout << "\nPlayer's Turn:" << std::endl;
    // Assuming simple console input for demonstration
    int row, col;
    do {
      std::cout << "Enter target coordinates (e.g., F2): ";
      char colChar;
      std::cin >> colChar >> row;
      col = colChar - 'A';
    } while (!isValidCoordinate(row, col, false));

    // Process player's move
    if (computerShipBoard[row][col] == SHIP) {
      std::cout << "Hit!" << std::endl;
      playerTargetBoard[row][col] = HIT;
      computerShipBoard[row][col] = HIT;
    } else {
      std::cout << "Miss!" << std::endl;
      playerTargetBoard[row][col] = MISS;
    }
  } else {
    // Computer's turn
    std::cout << "\nComputer's Turn:" << std::endl;
    int row, col;
    do {
      row = std::rand() % 10;
      col = std::rand() % 10;
    } while (!isValidCoordinate(row, col, true));

    // Process computer's move
    if (playerShipBoard[row][col] == SHIP) {
      std::cout << "Hit!" << std::endl;
      computerTargetBoard[row][col] = HIT;
      playerShipBoard[row][col] = HIT;
    } else {
      std::cout << "Miss!" << std::endl;
      computerTargetBoard[row][col] = MISS;
    }
  }

  // Display updated boards
  displayBoards();

  // Toggle the turn
  playerTurn = !playerTurn;
}

bool BattleshipGame::isGameOver() const {
  // Check if all player's ships are destroyed
  bool allPlayerShipsDestroyed = true;
  for (int i = 0; i < 10 && allPlayerShipsDestroyed; ++i) {
    for (int j = 0; j < 10; ++j) {
      if (computerShipBoard[i][j] == SHIP) {
        allPlayerShipsDestroyed = false;
        break;
      }
    }
  }

  // Check if all computer's ships are destroyed
  bool allComputerShipsDestroyed = true;
  for (int i = 0; i < 10 && allComputerShipsDestroyed; ++i) {
    for (int j = 0; j < 10; ++j) {
      if (playerShipBoard[i][j] == SHIP) {
        allComputerShipsDestroyed = false;
        break;
      }
    }
  }

  // The game is over if either player has all their ships destroyed
  return allPlayerShipsDestroyed || allComputerShipsDestroyed;
}

bool BattleshipGame::isValidCoordinate(int row, int col,
                                        bool isComputer) const {
  // Check if the given coordinate is valid for placing or attacking a ship
  if (row < 0 || row >= 10 || col < 0 || col >= 10) {
      std::cout << "Invalid coordinates. Try again." << std::endl;
      std::cin.clear();  // Clear the error state
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Discard invalid input
      return false;
  }

  if (isComputer && playerTargetBoard[row][col] != EMPTY) {
    std::cout << "Computer already targeted this location. Try again."
              << std::endl;
    return false;
  } else if (!isComputer && playerShipBoard[row][col] != EMPTY) {
    std::cout << "You already placed a ship here. Try again." << std::endl;
    return false;
  }

  return true;
}

void BattleshipGame::autoPlaceShips(std::vector<std::vector<char>> &board) {
  // Auto-place ships on the board using a random strategy
  // You need to implement this based on your specific rules
  for (const std::string &shipName : shipNames) {
    bool placed = false;
    while (!placed) {
      int orientation = std::rand() % 2; // 0 for horizontal, 1 for vertical
      int row = std::rand() % 10;
      int col = std::rand() % 10;

      if (orientation == 0) {
        if (col + shipName.length() <= 10) {
          placed = true;
          for (int i = 0; i < shipName.length(); ++i) {
            board[row][col + i] = SHIP;
          }
        }
      } else {
        if (row + shipName.length() <= 10) {
          placed = true;
          for (int i = 0; i < shipName.length(); ++i) {
            board[row + i][col] = SHIP;
          }
        }
      }
    }
  }
}
void BattleshipGame::setupGame() {
    std::cout << "Setting up the game..." << std::endl;

    // Allow the player to manually or auto-place their ships
    // You need to implement this based on your specific format

    // For demonstration purposes, auto-place ships
    autoPlaceShips(playerShipBoard);
    autoPlaceShips(computerShipBoard);

    // Display initial boards
    displayBoards();
}
int main() {
    BattleshipGame gameBoard;
    gameBoard.loadConfig("adaship_config.ini");
    gameBoard.setupGame();

    while (!gameBoard.isGameOver()) {
        gameBoard.playTurn();
    }

    return 0;
}