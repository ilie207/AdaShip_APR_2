#ifndef PLAYER_H
#define PLAYER_H

#include "Board.h"

class ComputerPlayer;
class Game;

class Player {
  friend class ComputerPlayer;
  friend class Game;

public:
  Player();
  void setup();
  void takeTurn(Player &opponent);
  bool isWinner() const;
  void printShipBoard() const;
  void printTargetBoard() const;
  bool isValidCoordinate(const std::string &coordinate) const;
  Board &getTargetBoard() { return this->targetBoard; }
  bool isTargeted(const std::string &coordinate) const;
  bool markHit(const std::string &targetCoordinate);

protected:
  Board shipBoard;
  Board targetBoard;
};

#endif