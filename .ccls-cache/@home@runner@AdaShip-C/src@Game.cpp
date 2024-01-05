#include <iostream>
#include <thread>

#include "../headers/Game.h"
#include "../headers/Board.h"
#include "../headers/ComputerPlayer.h"
#include "../headers/Player.h"

Game::Game() 
  : configLoader(ConfigLoader::getInstance()),
    player(computer) { 
    
ConfigLoader::getInstance().loadConfig(board);

std::pair<int, int> boardSize = ConfigLoader::getInstance().getBoardSize();
computer.initializeBoard(boardSize.first, boardSize.second);
player.initializeBoard(boardSize.first, boardSize.second);
}

Board board;

void Game::start() {
  computer.setPlayer(&player);

  const std::vector<std::pair<std::string, int>>& shipList = ConfigLoader::getInstance().getShipList();
  
  std::pair<int, int> boardSize = configLoader.getBoardSize();

  
  std::cout << "Welcome to Battleship Game!\n" << std::endl;
  std::cout << "Ship list:" << std::endl;
  for (const auto &ship : shipList) {
    std::cout << "Ship Name: " << ship.first 
      << " Length: " << ship.second << std::endl;
  }

  player.printBoards();
  computer.printBoards();

  std::cout << "\nPlease chose one of the options:\n";
  std::cout << "1. Player v Computer." << std::endl;
  std::cout << "2. Exit Game." << std::endl;
  int option;
  std::cin >> option;

   bool gameInProgress = true;
  
  switch (option) {
  case 1:
    char choice;
    do {
      // Prompt the player to place ships manually or randomly
      std::cout << "\nDo you want to place ships manually or randomly? (M/R): ";
      std::cin >> choice;

      if (toupper(choice) == 'M') {
        player.placeShipsManually();
      } else if (toupper(choice) == 'R') {
        player.placeShipsRandomly();
      } else {
        std::cout << "Invalid choice. Please try again." << std::endl;
        continue;
      }

      std::cout << "\nHere is your board with the ships placed:" << std::endl;
      player.printBoards();

      // Ask the player to confirm the ship placement
      std::cout << "Are you happy with this ship placement? (Y/N/R/Q): ";
        std::cin >> choice;
        if (toupper(choice) == 'R') {
          resetGame();
          break; // Return from start as resetGame calls start again
        } else if (toupper(choice) == 'Q') {
          quitGame();
          break; // Will not be reached if quit successfully executed
        } else if (toupper(choice) != 'Y') {
          player.resetBoards(boardSize);
        } 
      } while (toupper(choice) != 'Y');

    std::cout << "\nComputer is placing its ships on the board...\n";
    computer.placeComputerShipsRandomly();

    // Main game loop
    while (gameInProgress && !player.isGameOver()) {
      player.printBoards();
      computer.printBoards();
      player.playerTurn();
      if (player.isGameOver()) {
        break;
      }
      computer.computerTurn();
      // Check for player actions after each turn
      std::cout << "Enter 'R' to reset, 'Q' to quit, or any other key to continue: ";
      char playerChoice;
      std::cin >> playerChoice;
      switch(tolower(playerChoice)) {
        case 'r':
          resetGame();
          break; // Exit function since resetGame has called start again
        case 'q':
          quitGame();
          break; // No code should be executed after this point
        default: std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          break; // Continue playing
      }
    }
    // Display the final result
    player.printBoards();
    computer.printBoards();

    showWinner();
    std::cout << "Game Over!" << std::endl;
    break;

  case 2:
    quitGame();
    break;

  default:
    std::cout << "Invalid choice. Please try again." << std::endl;
    break;
  }
}

void Game::showWinner() {
  if (computer.checkWin(computer.computerTargetBoard,
    player.getPlayerBoard(), configLoader.getBoardSize())) {
    std::cout << "Computer has sunk all your ships. You lose!" 
      << std::endl;
  } else if (player.checkWin(player.playerTargetBoard,
    computer.computerBoard, configLoader.getBoardSize())) {
    std::cout << "Congratulations! You've sunk all enemy ships. You win!" << std::endl;
  }
}

void Game::resetGame() {
  // Clear previous ship list before loading the configuration again
  ConfigLoader::getInstance().clearShipList();

   // Get the current board size from the ConfigLoader
std::pair<int, int> boardSize = ConfigLoader::getInstance().getBoardSize();

  // Verify if shipList is actually cleared
  if (configLoader.getShipList().empty()) {
    std::cout << "\nShip list has been cleared.\n";
  } else {
    std::cout << "Warning: Ship list has NOT been cleared.\n";
  }

  // Reset player's and computer's boards
  player.resetBoards(boardSize);
  computer.resetBoards(boardSize);
  
  // Reload the configuration to repopulate the shipList
  ConfigLoader::getInstance().loadConfig(board);

  std::cout << "Game has been reset. Starting a new game...\n" << std::endl;
  start(); // Reinitialize the game setup
}

void Game::quitGame() {
  std::cout << "Quitting the game. Thank you for playing." << std::endl;
  exit(0);
}