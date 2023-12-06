#ifndef COMPUTER_H
#define COMPUTER_H

#include "Board.h"
#include "Player.h"
#include <vector>

class Computer {
public:
  Computer(Board &playerBoard);
  void placeComputerShipsRandomly();
  void updateComputerBoard(int row, int col, bool &hit, bool &shipSunk);

private:
  Board playerBoard;
  Board computerBoard;
  Board computerTargetBoard;
};

#endif