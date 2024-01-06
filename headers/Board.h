#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <vector>

const char EMPTY_CELL = '.';
const char HIT_CELL = 'X';
const char MISS_CELL = 'O';
const char SHIP_SUNK = 'S';

class Board {
public:
  virtual void printBoards() const;

  void printBoard(const std::vector<std::vector<char>>& board,
  std::pair<int, int> boardSize) const;
  
  bool isValidCoordinate(int row, int col, 
  std::pair<int, int> boardSize);
  
  bool isValidTarget(int row, int col, 
  const std::vector<std::vector<char>>& board,
  std::pair<int, int> boardSize);

  bool checkWin(const std::vector<std::vector<char>>& targetBoard, 
  const std::vector<std::vector<char>>& referenceBoard, 
  std::pair<int, int> boardSize);

  bool isValidPlacement(int row, int col, int length, bool 
  horizontal, const std::vector<std::vector<char>> &board, 
  std::pair<int, int> boardSize);

  void updateBoard(std::vector<std::vector<char>> &board, int row, 
  int col, int length, bool horizontal, char symbol);

  void resetBoards(std::pair<int,int> boardSize);

  std::vector<std::pair<std::string, int>> shipList;
  std::vector<std::vector<char>> playerBoard;
  std::vector<std::vector<char>> playerTargetBoard;
  std::vector<std::vector<char>> computerBoard;
  std::vector<std::vector<char>> computerTargetBoard;
  void initializeBoard(int rows, int cols);

private:
};
#endif