#ifndef SEQUENCER_HPP
#define SEQUENCER_HPP

#include "tokenizer.hpp"
#include <Eigen/Geometry>
#include <map>
#include <queue>
#include <string>
#include <vector>

class Sequencer {
private:
  std::vector<int> key_string;
  std::map<int, std::string> token_lut;
  uint idx = 0;
  float offset[2];
  float h_off;
  float v_off;
  float spacing;
  float scale;
  float max_hlen;
  float max_vlen;
  float rot_angle;
  uint space_flag;
  Eigen::Matrix2d rot;
  std::string ref;
  void initTokenMap(std::map<int, std::string> tokenMap);
  void queryLUT();
  int applyTransform(Token *curr_tok);

public:
  Sequencer(const std::string &input, std::map<int, std::string> tokenMap,
            float h_off, float v_off, float scale, float spacing,
            float max_hlen, float max_vlen, float rot_angle = 90);
  std::queue<Token> send_buffer;
  uint Step();
};

#endif // !SEQUENCER_HPP
