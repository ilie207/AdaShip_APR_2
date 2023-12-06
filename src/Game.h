#ifndef GAME_H
#define GAME_H

#include "ComputerPlayer.h"
#include "Player.h"

class Game {
public:
  Game();
  void run();

private:
  Player humanPlayer;
  ComputerPlayer computerPlayer;
  bool isGameOver;

  void printBoards();
  void playerTurn();
  void computerTurn();
  bool checkGameOver();
};

#endif // GAME_H
