#include "Player.h"
#include <iomanip>
#include <iostream>
#include <string>

Player::Player() {
  // Constructor
}

void Player::setup() {
  std::cout << "Setting up your ships..." << std::endl;

  // For simplicity, auto-place ships
  shipBoard.autoPlaceShips();

  // Display the initial shipboard
  std::cout << "Your Shipboard:" << std::endl;
  shipBoard.printBoard();
}

void Player::takeTurn(Player &opponent) {
  std::cout << "It's your turn!" << std::endl;

  printTargetBoard();

  // Player fires at a target
  std::string targetCoordinate;
  do {
    std::cout << "Enter target coordinate (e.g., A1, B2): ";
    std::cin >> targetCoordinate;
  } while (!isValidCoordinate(targetCoordinate) ||
           targetBoard.isTargeted(targetCoordinate));

  // Mark the target on the opponent's shipboard
  bool isHit = opponent.shipBoard.markHit(targetCoordinate);
  targetBoard.markTarget(targetCoordinate, isHit);

  // Display the result of the turn
  std::cout << "Your Turn Result:" << std::endl;
  targetBoard.printBoard();

  // Check for a win condition
  if (opponent.shipBoard.allShipsSunk()) {
    std::cout << "Congratulations! You've sunk all of the opponent's ships!"
              << std::endl;
  }
}

bool Player::markHit(const std::string &coordinate) {
  // Check if the coordinate has a valid format
  if (!isValidCoordinate(coordinate)) {
    return false;
  }

  // Extract row and column from the coordinate
  char col = coordinate[0];
  int row = coordinate[1] - '0' - 1; // Adjust to zero-based index

  // Check if the target is a valid cell on the shipboard
  if (row < 0 || row >= 10 || col < 'A' || col - 'A' >= 10) {
    return false;
  }

  // Check if the cell is already marked as a hit or miss
  if (shipBoard.targetGrid[row][col - 'A'] == 'H' ||
      shipBoard.targetGrid[row][col - 'A'] == 'M') {
    std::cout << "You've already targeted this coordinate." << std::endl;
    return false; // Cell has already been targeted
  }

  // Mark the target on the opponent's shipboard
  bool isHit = shipBoard.markHit(coordinate);

  // Mark the target on the player's target board
  targetBoard.markTarget(coordinate, isHit);

  return true; // Hit was successful
}

bool Player::isWinner() const {
  // Check for a win condition
  return shipBoard.allShipsSunk();
}

void Player::printShipBoard() const {
  std::cout << "Your Shipboard:" << std::endl;
  shipBoard.printBoard();
}

void Player::printTargetBoard() const {
  std::cout << "Your Targetboard:" << std::endl;
  targetBoard.printBoard();
}

bool Player::isValidCoordinate(const std::string &coordinate) const {
  // Check if the coordinate has a valid format (e.g., A1, B2)
  if (coordinate.length() != 2) {
    return false;
  }

  char col = coordinate[0];
  int row = coordinate[1] - '0';

  return (col >= 'A' && col <= 'J') && (row >= 1 && row <= 10);
}

bool Player::isTargeted(const std::string &coordinate) const {
  // Check if the coordinate has been targeted on the targetBoard
  return targetBoard.isTargeted(coordinate);
}
