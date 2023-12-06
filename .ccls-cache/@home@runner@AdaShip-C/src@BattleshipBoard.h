// BattleshipGame.h
#ifndef BATTLESHIP_GAME_H
#define BATTLESHIP_GAME_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>

const char EMPTY = '~';
const char SHIP = 'O';
const char HIT = 'X';
const char MISS = '-';

class BattleshipGame {
private:
std::vector<std::vector<char>> playerShipBoard;
std::vector<std::vector<char>> playerTargetBoard;
std::vector<std::vector<char>> computerShipBoard;
std::vector<std::vector<char>> computerTargetBoard;
bool playerTurn;
bool gameOver;
std::vector<std::string> shipNames;

public:
    BattleshipGame();
    void loadConfig(const std::string& configFileName);
    void displayBoards() const;
    void setupGame();
    void playTurn();
    bool isGameOver() const;
    bool isValidCoordinate(int row, int col, bool isComputer) const;
    void autoPlaceShips(std::vector<std::vector<char>>& board);
};

#endif
