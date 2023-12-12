#include <iostream>

#include "../headers/Game.h"
#include "../headers/Board.h"
#include "../headers/ComputerPlayer.h"
#include "../headers/Player.h"

Game::Game() : configLoader(ConfigLoader::getInstance()) {
}

void Game::start() {
  Board board;
  ConfigLoader::getInstance().loadConfig(board);

  const std::vector<std::pair<std::string, int>>& shipList = ConfigLoader::getInstance().getShipList();


  std::cout << "Debug: shipList inside game file:" << std::endl;
  for (const auto &ship : shipList) {
    std::cout << "Ship Name: " << ship.first << ", Length: " << ship.second
              << std::endl;
  }

  Player player;
  ComputerPlayer computer;

  std::cout << "Welcome to Battleship Game!" << std::endl;
  std::cout << "Please chose one of the options:" << std::endl;
  std::cout << "1. Player v Computer." << std::endl;
  std::cout << "2. Exit Game." << std::endl;
  int option;
  std::cin >> option;

  switch (option) {
  case 1:
    // Prompt the player to place ships manually or randomly
    std::cout << "Do you want to place ships manually or randomly? (M/R): ";
    char choice;
    std::cin >> choice;

    if (toupper(choice) == 'M' || toupper(choice) == 'm') {
      player.placeShipsManually();
    } else if (toupper(choice) == 'R' || toupper(choice) == 'r') {
      player.placeShipsRandomly();
    } else {
      std::cout << "Invalid choice. Please try again." << std::endl;
    }

    std::cout << "\nComputer is placing its ships on the board...\n";
    computer.placeComputerShipsRandomly();
    // Main game loop
    while (!board.isGameOver()) {
      board.printBoards();
      player.playerTurn();
      if (!board.isGameOver()) { // Check if the game is still not over before
                                 // the computer's turn
        computer.computerTurn();
      }
    }

    // Display the final result
    board.printBoards();
    std::cout << "Game Over!" << std::endl;
    break;

  case 2:
    std::cout << "Exiting game..." << std::endl;
    return;
    break;

  default:
    std::cout << "Invalid choice. Please try again." << std::endl;
    break;
  }

  // Display the final result
  board.printBoards();
  std::cout << "Game Over!" << std::endl;
}