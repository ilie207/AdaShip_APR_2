#ifndef PLAYER_H
#define PLAYER_H

#include "Board.h"
#include <map>
#include <string>
#include <vector>

class ComputerPlayer;

class Player : public Board{

public:  
  explicit Player(ComputerPlayer& computerPlayer);

  void placeShipsManually();
  void placeShipsRandomly();
  void placeShipRandomly(const std::string& shipName, int length);
  void updatePlayerBoard(int row, int col, bool &hit, bool &shipSunk);
  void playerTurn();
  void printBoards() const override;
  const std::vector<std::vector<char>>& getPlayerBoard() const; 
  bool isGameOver();


private:
  ComputerPlayer& computerPlayer; 
};
#endif