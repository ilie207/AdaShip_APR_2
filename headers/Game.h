#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "ConfigLoader.h"
#include "ComputerPlayer.h"
#include "Player.h"

class Game{
public:
  Game();
  void start();
  void showWinner();
  void resetGame();
  void quitGame();
  void clearShipList();

private:
ConfigLoader configLoader;
};

#endif
