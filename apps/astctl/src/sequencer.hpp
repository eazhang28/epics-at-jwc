#ifndef SEQUENCER_HPP
#define SEQUENCER_HPP

#include "tokenizer.hpp"
#include <map>
#include <string>
#include <vector>

class Sequencer {
private:
  std::vector<int> key_string;
  std::map<int, std::string> token_lut;
  uint idx = 0;
  float offset[2] = {0, 0};
  float spacing = 5;
  float scale = 1;
  float max_hlen = 100;
  float max_vlen = 200;
  std::string ref;
  void initTokenMap(std::map<int, std::string> tokenMap);
  void queryLUT();
  void applyTransform(Token *curr_tok);

public:
  Sequencer(const std::string &input, std::map<int, std::string> tokenMap,
            float h_off, float v_off, float scale, float spacing);
  uint Step();
};

#endif // !SEQUENCER_HPP
