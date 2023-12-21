#include "../headers/ConfigLoader.h"
#include "../headers/Board.h"
#include <fstream>
#include <iostream>
#include <sstream>

ConfigLoader::ConfigLoader() {}

ConfigLoader &ConfigLoader::getInstance() {
  static ConfigLoader instance;
  return instance;
}

void ConfigLoader::loadConfig(Board &board) {

  std::ifstream configFile("adaship_config.ini");

  if (!configFile.is_open()) {
    std::cerr << "Error: Unable to open configuration file." << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string line;
  while (std::getline(configFile, line)) {
    std::istringstream iss(line);
    std::string key;
    iss >> key;

    if (key == "Board:") {
      // Parse board size
      int rows, cols;
      char x; // to store the 'x' between rows and columns
      iss >> rows >> x >> cols;
      boardSize = {rows, cols};

      // Set up the game board with the specified dimensions
      board.initializeBoard(rows, cols);
    } else if (key == "Boat:") {
      // Parse boat information
      std::string boatName;
      int boatLength;
      iss >> boatName >> boatLength;

      // Validate boat length
      if (boatLength <= 0) {
        std::cerr << "Error: Invalid boat length for " << boatName << std::endl;
        exit(EXIT_FAILURE);
      }

      // Add boat information to shipList
      shipList.emplace_back(boatName, boatLength);
    }
  }

  configFile.close();
}

const std::vector<std::pair<std::string, int>> &
ConfigLoader::getShipList() const {
  return shipList;
}

std::pair<int, int> ConfigLoader::getBoardSize() const {
  return boardSize;
}