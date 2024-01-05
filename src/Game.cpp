#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <thread>
#include <limits>

#include "../headers/Game.h"
#include "../headers/Board.h"
#include "../headers/ComputerPlayer.h"
#include "../headers/Player.h"

Game::Game() : configLoader(ConfigLoader::getInstance()), player(computer) { 
ConfigLoader::getInstance().loadConfig(board);
// Initialising the boards
std::pair<int, int> boardSize = ConfigLoader::getInstance().getBoardSize();
computer.initializeBoard(boardSize.first, boardSize.second);
player.initializeBoard(boardSize.first, boardSize.second);
}

Board board;

void Game::start() {
  computer.setPlayer(&player);

  const std::vector<std::pair<std::string, int>>& shipList = ConfigLoader::getInstance().getShipList();


  std::cout << "Welcome to Battleship Game!\n" << std::endl;
  std::cout << "Ship list:" << std::endl;
  for (const auto &ship : shipList) {
    std::cout << "Ship Name: " << ship.first 
      << " Length: " << ship.second << std::endl;
  }

while(true) {
  std::cout << "\nPlease chose one of the options:\n";
  std::cout << "1. Player v Computer." << std::endl;
  std::cout << "2. Exit Game." << std::endl;
  std::string input;
  std::getline(std::cin, input);

  if(input.find(" ") != std::string::npos){
    std::cout 
      << "Invalid input. Please DO NOT include space!\n";
    continue;
  }

  int option = 0;
  std::istringstream(input) >> option;

  switch (option) {
  case 1:
    shipPlacement();
    playerVsComputer();
    break;

  case 2:
    quitGame();
    break;

  default:
    std::cout << "Invalid choice. Please choose 1 or 2." 
      << std::endl;
    break;
  }
 }
}

/*void Game::showWinner() {
  if (computer.checkWin(computer.computerTargetBoard,
    player.getPlayerBoard(), configLoader.getBoardSize())) {
    std::cout << "Computer has sunk all your ships. You lose!" 
      << std::endl;
  } else if (player.checkWin(player.playerTargetBoard,
    computer.computerBoard, configLoader.getBoardSize())) {
    std::cout << "Congratulations! You've sunk all enemy ships. You win!" << std::endl;
  }
}*/

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

void Game::playerVsComputer() {
    // Main game loop
    while (!player.isGameOver()) {
      player.printBoards();
      computer.printBoards();
      player.playerTurn();
      if (player.isGameOver()) {
        break;
      }
      computer.computerTurn();
      if (player.isGameOver()) {
          break;
        }

        // Check for player actions after each turn
        std::cout << "Enter 'R' to reset, 'Q' to quit, or any other key to continue: ";           std::string playerInput;
          std::cin.ignore();
          std::cin >> playerInput;
          // Trim input for accidental spaces by users
          playerInput.erase(0, playerInput.find_first_not_of(" \n\r\t"));
          playerInput.erase(playerInput.find_last_not_of(" \n\r\t") + 1);
          
          if(playerInput.empty()) {
            std::cout << "No choice entered.\n";
            continue;
          }
          char playerChoice = toupper(playerInput[0]);
          switch(playerChoice) {
            case 'R':
              resetGame();
              break; // Exit function since resetGame has called start again
            case 'Q':
              quitGame();
              return; // No code should be executed after this point
            default:
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
              break; // Continue playing
          }
      }

      // Display the final result
      player.printBoards();
      computer.printBoards();

      // If the game is over, show the winner
      if (player.isGameOver()) {
        if (player.checkWin(
          player.playerTargetBoard, 
          computer.computerBoard, 
          configLoader.getBoardSize())) {
          std::cout << "Congratulations! You've sunk all enemy ships. You win!" 
            << std::endl;
        } else {
          std::cout << "Computer has sunk all your ships. You lose!" 
            << std::endl;
        }
      }
  
      std::cout << "Game Over!" << std::endl;
      quitGame();
      
    }

void Game::shipPlacement() {
  std::pair<int, int> boardSize = ConfigLoader::getInstance().getBoardSize();
  
  bool validInput = false;
  
    while (!validInput){
      // Prompt the player to place ships manually or randomly
      std::cout << "\nDo you want to place ships manually or randomly? (M/R): ";
      std::string input;
      std::getline(std::cin, input);

      // Trim input for accidental spaces by users
      input.erase(0, input.find_first_not_of(" \n\r\t"));
      input.erase(input.find_last_not_of(" \n\r\t") + 1);

      if(input.length() != 1){
        std::cout << "Invalid choice. Please enter 'M' or 'R'.\n";
        continue;
      }

      char choice = toupper(input[0]);

      if (toupper(choice) == 'M') {
        player.placeShipsManually();
        validInput = true;
      } else if (toupper(choice) == 'R') {
        player.placeShipsRandomly();
        validInput = true;
      } else {
        std::cout << "Invalid choice. Please enter 'M' or 'R'." << std::endl;
        continue;
      }
    }

    validInput = false;
    while (!validInput){
      std::cout << "\nHere is your board with the ships placed:" << std::endl;
      player.printBoards();

      // Ask the player to confirm the ship placement
      std::cout << "Are you happy with this ship placement? (Y/N/R/Q): ";
      std::string resetInput;
      std::getline(std::cin, resetInput);

        // Trim input for accidental spaces by users
        resetInput.erase(0, resetInput.find_first_not_of(" \n\r\t"));
        resetInput.erase(resetInput.find_last_not_of(" \n\r\t") + 1);

        if(resetInput.length() != 1){
        std::cout << "Invalid input. Please enter a single character!\n";
        continue;
      }

      char decision = toupper(resetInput[0]);
      std::istringstream(resetInput) >> decision;
      
        switch (toupper(decision)) {
          case 'Y':
            validInput = true;
            break;
          case 'N':
            validInput = false;
            player.resetBoards(boardSize);
            computer.resetBoards(boardSize);
            break;
          case 'R':
            resetGame();
            validInput = true; // After resetGame, the start function will loop again
            break;
          case 'Q':
            quitGame();
            validInput = true; // Not reached if quitGame exits
            break;
          default:
           std::cout << "Invalid choice. Please choose one of the options:" 
             << std::endl;
           std::cout << "'Y' to confirm,\n" 
             << "'N' to reset the placements,\n" 
             << "'R' to reset and start again,\n" 
             << "'Q' to quit." << std::endl; 
      
            validInput = false;
            break;
        }
      }
      std::cout << "\nComputer is placing its ships on the board...\n";
      computer.placeComputerShipsRandomly();
}