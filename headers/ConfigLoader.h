#include "Board.h"
#include <string>
#include <vector>

class ConfigLoader {
public:
  static ConfigLoader &getInstance();

  void loadConfig(Board &board);
  const std::vector<std::pair<std::string, 
  int>> &getShipList() const;
  std::pair<int, int> getBoardSize() const;
   void clearShipList();

private:
  ConfigLoader();
  std::vector<std::pair<std::string, int>> shipList;
  std::pair<int, int> boardSize;
};
