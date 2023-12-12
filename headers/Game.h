#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "ConifgLoader.h"
#include "ComputerPlayer.h"
#include "Player.h"

class Game{
public:
  Game();
  void start();

private:
ConfigLoader configLoader;
};

#endif
