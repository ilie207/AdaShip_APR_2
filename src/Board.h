#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <vector>

class Board {
public:
  Board();
  void autoPlaceShips();
  void autoFire(Board &opponentBoard);
  bool markHit(const std::string &coordinate);
  bool isTargeted(const std::string &coordinate) const;
  bool allShipsSunk() const;
  void printBoard() const;
  bool markTarget(const std::string &coordinate, bool isHit);
  void placeShipRandomly(int length, char shipSymbol);
  bool isShipSunk(const std::string &coordinate) const;
  void convertToRowCol(const std::string &coordinate, int &row, int &col) const;
  std::string convertToCoordinate(int row, int col) const;
  std::vector<std::vector<char>> targetGrid;

private:
  std::vector<std::vector<char>> shipGrid;
  std::vector<std::string> shipCoordinates;
};

#endif