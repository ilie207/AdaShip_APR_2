#include "Board.h"
#include <string>
#include <vector>

class ConfigLoader {
public:
  static ConfigLoader &getInstance();

  void loadConfig(Board &board);
  const std::vector<std::pair<std::string, int>> &getShipList() const;

private:
  ConfigLoader();
  std::vector<std::pair<std::string, int>> shipList;
};
