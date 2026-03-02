#ifndef SEQUENCER_HPP
#define SEQUENCER_HPP

#include "sqlut.hpp"
#include <map>
#include <set>
#include <string>
#include <vector>

class Sequencer {
private:
  std::vector<int> key_string;
  std::map<int, std::string> token_lut;
  uint idx = 0;
  void initTokenMap(std::map<int, std::string> tokenMap);
  void queryLUT();

public:
  Sequencer(const std::string &input, std::map<int, std::string> tokenMap);
  std::map<int, std::string> getLUT() const;
  uint Step();
};

#endif // !SEQUENCER_HPP
