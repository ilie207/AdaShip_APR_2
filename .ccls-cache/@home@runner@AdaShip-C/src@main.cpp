#include <iostream>

//#include "../headers/ComputerPlayer.h"
#include "../headers/Game.h"

int main() {
  srand(static_cast<unsigned>(time(nullptr)));

  Game game;

  game.start();

  return 0;
}
