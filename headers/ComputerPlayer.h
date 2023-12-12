#ifndef COMPUTERPLAYER_H
#define COMPUTERPLAYER_H

#include "../headers/Board.h"
#include "../headers/Player.h"

class ComputerPlayer : public Board {
public:
  // ComputerPlayer();

  void placeComputerShipsRandomly();
  void computerTurn();
  void updateComputerBoard(int row, int col, bool &hit, bool &shipSunk);

private:
  std::vector<std::pair<std::string, int>> shipList;
};

#endif