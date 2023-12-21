#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <vector>

const int BOARD_SIZE = 10;
const char EMPTY_CELL = '.';
const char HIT_CELL = 'X';
const char MISS_CELL = 'O';

class Board {
public:
  void loadConfig();
  virtual void printBoards() const;
  void printBoard(const std::vector<std::vector<char>>& board) const;
  bool isValidCoordinate(int row, int col);
  bool isValidTarget(int row, int col);
  bool checkWin(const std::vector<std::vector<char>>& targetBoard, const std::vector<std::vector<char>>& referenceBoard);
  bool isValidPlacement(int row, int col, int length, 
  bool horizontal, const std::vector<std::vector
  <char>> &board);
  void updateBoard(std::vector<std::vector<char>> &board,
  int row, int col, int length, bool horizontal, char symbol);

  std::vector<std::pair<std::string, int>> shipList;
  std::vector<std::vector<char>> playerBoard;
  std::vector<std::vector<char>> playerTargetBoard;
  std::vector<std::vector<char>> computerBoard;
  std::vector<std::vector<char>> computerTargetBoard;
  void initializeBoard(int rows, int cols);

private:
};

#endif