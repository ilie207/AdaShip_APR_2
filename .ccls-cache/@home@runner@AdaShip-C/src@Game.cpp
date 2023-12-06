#include "Game.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

Game::Game() : isGameOver(false) {
  // Initialize players and setup the game
  humanPlayer.setup();
  computerPlayer.setup();
}

void Game::run() {
  while (!isGameOver) {
    printBoards();
    playerTurn();
    isGameOver = checkGameOver();
    if (isGameOver)
      break;

    printBoards();
    computerTurn();
    isGameOver = checkGameOver();
  }

  // Display the winner
  if (humanPlayer.isWinner()) {
    std::cout << "Congratulations! You won!" << std::endl;
  } else {
    std::cout << "Computer wins. Better luck next time!" << std::endl;
  }
}

void Game::printBoards() {
  // Display the current state of the boards
  std::cout << "Your Shipboard:" << std::endl;
  humanPlayer.printShipBoard();

  std::cout << "Your Targetboard:" << std::endl;
  humanPlayer.printTargetBoard();

  std::cout << "Computer's Targetboard:" << std::endl;
  computerPlayer.printTargetBoard();
}

void Game::playerTurn() {
  std::cout << "Your Turn!" << std::endl;
  printBoards(); // Display the boards before the turn

  std::string targetCoordinate;
  do {
    std::cout << "Enter target coordinate (e.g., A1, B2): ";
    std::cin >> targetCoordinate;
  } while (!humanPlayer.isValidCoordinate(targetCoordinate) ||
           humanPlayer.targetBoard.isTargeted(targetCoordinate));

  // Mark the target on the computer's shipboard
  bool isHit = computerPlayer.shipBoard.markHit(targetCoordinate);
  bool playerHit = humanPlayer.targetBoard.markTarget(targetCoordinate, isHit);

  // Display the result of the turn
  std::cout << "Your Turn Result:" << std::endl;
  printBoards();
  if (playerHit) {
    std::cout << "Hit!" << std::endl;
  } else {
    std::cout << "Miss!" << std::endl;
  }

  // Check for a win condition
  if (computerPlayer.shipBoard.allShipsSunk()) {
    std::cout << "Congratulations! You've sunk all of the computer's ships!"
              << std::endl;
    isGameOver = true;
  }
}

void Game::computerTurn() {
  std::cout << "Computer's Turn!" << std::endl;
  computerPlayer.takeTurn(humanPlayer);
}

bool Game::checkGameOver() {
  // Check if either player has won
  return humanPlayer.isWinner() || computerPlayer.isWinner();
}
