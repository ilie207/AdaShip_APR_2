#include "Game.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

int main() {
  // Display the menu for the user to choose the game mode
  std::cout << "Welcome to BattleShip!" << std::endl;
  std::cout << "1. One Player vs Computer" << std::endl;
  std::cout << "2. Quit" << std::endl;

  int choice;
  std::cout << "Enter your choice (1 or 2): ";
  std::cin >> choice;

  switch (choice) {
  case 1: {
    // Start a one-player vs computer game
    Game battleShipGame;
    battleShipGame.run();
    break;
  }
  case 2: {
    // Quit the game
    std::cout << "Quitting the game. Goodbye!" << std::endl;
    break;
  }
  default: {
    // Invalid choice
    std::cout << "Invalid choice. Please enter 1 or 2." << std::endl;
    break;
  }
  }

  return 0;
}
