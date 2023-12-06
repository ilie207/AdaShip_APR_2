#ifndef BATTLESHIP_BOARD_H
#define BATTLESHIP_BOARD_H

#include <iostream>
#include <string>
#include <vector>

class BattleshipBoard {
private:
  std::vector<std::vector<char>> playerShipBoard;
  std::vector<std::vector<char>> playerTargetBoard;
  std::vector<std::vector<char>> computerShipBoard;
  std::vector<std::vector<char>> computerTargetBoard;
  std::vector<std::string> shipNames;
  bool playerTurn;
  bool gameOver;static 


public:
  BattleshipBoard() noexcept;
  void loadConfig(const std::string &configFileName);
  void displayBoards() const;
  void setupGame();
  void playTurn();
  bool isGameOver() const;
  bool isValidCoordinate(int row, int col, bool isComputer) const;
  void autoPlaceShips(std::vector<std::vector<char>> &board);
};

#endif // BATTLESHIP_BOARD_H
