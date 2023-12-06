#include "Board.h"
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

Board::Board() {
  // Initialize shipGrid and targetGrid
  shipGrid.resize(10, std::vector<char>(10, '.'));
  targetGrid.resize(10, std::vector<char>(10, '.'));
}

void Board::autoPlaceShips() {
  // For simplicity, auto-place ships at random locations
  std::srand(std::time(nullptr)); // Seed for randomization

  // Place Carrier (length 5)
  placeShipRandomly(5, 'C');

  // Place Battleship (length 4)
  placeShipRandomly(4, 'B');

  // Place Destroyer (length 3)
  placeShipRandomly(3, 'D');

  // Place Submarine (length 3)
  placeShipRandomly(3, 'S');

  // Place Patrol Boat (length 2)
  placeShipRandomly(2, 'P');
}

void Board::autoFire(Board &opponentBoard) {
  // For simplicity, auto-fire at random locations
  int row = std::rand() % 10;
  int col = std::rand() % 10;
  std::string targetCoordinate = convertToCoordinate(row, col);

  // Check if the coordinate has been targeted before
  while (opponentBoard.isTargeted(targetCoordinate)) {
    row = std::rand() % 10;
    col = std::rand() % 10;
    targetCoordinate = convertToCoordinate(row, col);
  }

  // Mark the target on the opponent's shipboard
  bool isHit = opponentBoard.markHit(targetCoordinate);
  markTarget(targetCoordinate, isHit);
}

bool Board::markHit(const std::string &coordinate) {
  // Mark the hit on the shipGrid and return true if a ship is hit
  int row, col;
  convertToRowCol(coordinate, row, col);

  if (shipGrid[row][col] != '.') {
    // Mark the hit on the shipGrid
    shipGrid[row][col] = 'X';

    // Check if the entire ship is sunk
    if (isShipSunk(coordinate)) {
      std::cout << "You sunk a ship!" << std::endl;
    }

    return true;
  } else {
    return false;
  }
}

bool Board::markTarget(const std::string &coordinate, bool isHit) {
  // Mark the target on the targetGrid
  int row, col;
  convertToRowCol(coordinate, row, col);
  targetGrid[row][col] = (isHit) ? 'H' : 'M';
  return isHit;
}

bool Board::isTargeted(const std::string &coordinate) const {
  // Check if the coordinate has been targeted before
  int row, col;
  convertToRowCol(coordinate, row, col);
  return (targetGrid[row][col] == 'H' || targetGrid[row][col] == 'M');
}

bool Board::allShipsSunk() const {
  // Check if all ships are sunk
  for (const auto &row : shipGrid) {
    for (char cell : row) {
      if (cell != 'O' && cell != 'X') {
        return false;
      }
    }
  }
  return true;
}

void Board::printBoard() const {
  // Print the board for display
  std::cout << "   A B C D E F G H I J" << std::endl;
  for (int i = 0; i < 10; ++i) {
    std::cout << std::setw(2) << i + 1 << " ";
    for (int j = 0; j < 10; ++j) {
      std::cout << shipGrid[i][j] << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

void Board::placeShipRandomly(int length, char shipSymbol) {
  // Place a ship of given length randomly on the shipGrid
  int row, col;
  bool isHorizontal = (std::rand() % 2 == 0);

  if (isHorizontal) {
    // Place horizontally
    row = std::rand() % 10;
    col = std::rand() % (10 - length + 1);
  } else {
    // Place vertically
    row = std::rand() % (10 - length + 1);
    col = std::rand() % 10;
  }

  for (int i = 0; i < length; ++i) {
    if (isHorizontal) {
      shipGrid[row][col + i] = shipSymbol;
      shipCoordinates.push_back(convertToCoordinate(row, col + i));
    } else {
      shipGrid[row + i][col] = shipSymbol;
      shipCoordinates.push_back(convertToCoordinate(row + i, col));
    }
  }
}

bool Board::isShipSunk(const std::string &coordinate) const {
  // Check if the entire ship is sunk
  char shipSymbol = shipGrid[coordinate[1] - '0' - 1][coordinate[0] - 'A'];
  for (const std::string &shipCoord : shipCoordinates) {
    int row, col;
    convertToRowCol(shipCoord, row, col);
    if (shipGrid[row][col] == shipSymbol) {
      return false; // The ship is not entirely sunk
    }
  }
  return true; // The ship is entirely sunk
}

void Board::convertToRowCol(const std::string &coordinate, int &row,
                            int &col) const {
  // Convert the coordinate (e.g., A1) to row and column indices
  col = coordinate[0] - 'A';
  row = coordinate[1] - '0' - 1;
}

std::string Board::convertToCoordinate(int row, int col) const {
  // Convert row and column indices to a coordinate (e.g., A1)
  char colChar = col + 'A';
  return colChar + std::to_string(row + 1);
}
