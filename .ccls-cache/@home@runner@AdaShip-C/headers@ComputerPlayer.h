#ifndef COMPUTERPLAYER_H
#define COMPUTERPLAYER_H

#include "../headers/Board.h"
#include "../headers/Player.h"

class ComputerPlayer : public Board {
public:
  ComputerPlayer();

  void placeComputerShipsRandomly();
  void computerTurn();
  void updateComputerBoard(int row, int col, bool &hit, bool &shipSunk);
  void printBoards() const override;
  void setPlayer(Player* player);
  std::vector<std::pair<std::string, int>> shipList;


private:
  Player* player;
};

#endif