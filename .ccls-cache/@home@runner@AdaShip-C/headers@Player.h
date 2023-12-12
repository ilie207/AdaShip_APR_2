#ifndef PLAYER_H
#define PLAYER_H

#include "Board.h"
#include <map>
#include <string>
#include <vector>

class Player : public Board{

public:
 std::vector<std::pair<std::string, int>> shipList;  
  Player();
  void placeShipsManually();
  void placeShipsRandomly();
  void updatePlayerBoard(int row, int col, bool &hit, bool &shipSunk);
  void playerTurn();

private:
//Board playerBoard;
};
#endif