#include <iostream>

#include "../headers/Game.h"
#include "../headers/Board.h"
#include "../headers/ComputerPlayer.h"
#include "../headers/Player.h"

Game::Game() : configLoader(ConfigLoader::getInstance()) {
  // Additional initialization for the Game class if needed
}

void Game::start() {
  Board board;
  ComputerPlayer computer;
  Player player(computer);
  computer.setPlayer(&player);

  ConfigLoader::getInstance().loadConfig(board);

  const std::vector<std::pair<std::string, int>>& shipList = ConfigLoader::getInstance().getShipList();


  std::cout << "Welcome to Battleship Game!\n" << std::endl;
  std::cout << "Ship list:" << std::endl;
  for (const auto &ship : shipList) {
    std::cout << "Ship Name: " << ship.first 
      << "Length: " << ship.second << std::endl;
  }


  std::cout << "\nPlease chose one of the options:\n";
  std::cout << "1. Player v Computer." << std::endl;
  std::cout << "2. Exit Game." << std::endl;
  int option;
  std::cin >> option;

  switch (option) {
  case 1:
    // Prompt the player to place ships manually or randomly
    std::cout << "\nDo you want to place ships manually or randomly? (M/R): ";
    char choice;
    std::cin >> choice;

    if (toupper(choice) == 'M' || 
      toupper(choice) == 'm') {
      player.placeShipsManually();
    } else if (toupper(choice) == 'R' ||
      toupper(choice) == 'r') {
      player.placeShipsRandomly();
    } else {
      std::cout << "Invalid choice. Please try again." 
        << std::endl;
    }

    std::cout << "\nComputer is placing its ships on the board...\n";
    computer.placeComputerShipsRandomly();

    // Main game loop
    while (!player.isGameOver()) {
      player.printBoards();
      computer.printBoards();
      player.playerTurn();
      if (!player.isGameOver()) {
        computer.computerTurn();
      }
    }
    // Display the final result
    player.printBoards();
    computer.printBoards();

    if (computer.checkWin(computer.computerTargetBoard,
      player.getPlayerBoard())) {
      std::cout << "Computer has sunk all your ships. You lose!"
        << std::endl;
    } else if (player.checkWin(player.playerTargetBoard,
      computer.computerBoard)) {
      std::cout << "Congratulations! You've sunk all enemy ships. You win!" 
        << std::endl;
    }
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
}