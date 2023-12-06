#ifndef COMPUTERPLAYER_H
#define COMPUTERPLAYER_H

#include "Player.h"

class ComputerPlayer : public Player {
public:
  ComputerPlayer();

  void autoSetup();
  void makeSmartMove(Player &opponent);

private:
  bool isValidSmartMove(int row, int col, const Player &opponent) const;
  std::string generateRandomCoordinate() const;
};

#endif