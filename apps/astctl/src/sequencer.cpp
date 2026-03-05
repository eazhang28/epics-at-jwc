#include "sequencer.hpp"

#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <tokenizer.hpp>
// sequencer will track offset, newline, whitespace, font, token cache, and
// input string
//
//

Sequencer::Sequencer(const std::string &input,
                     std::map<int, std::string> tokenMap, float h_off,
                     float v_off, float scale, float spacing) {
  this->token_lut = tokenMap;
  this->offset[0] = h_off, this->offset[1] = v_off;
  this->scale = scale;
  this->spacing = spacing;
  this->ref = input;
}

void Sequencer::applyTransform(Token *curr_tok) {
  Eigen::MatrixXd mat;
  mat = *curr_tok * scale;
  mat = *curr_tok + offset;
  std::string str = curr_tok->matrix_to_gcode(mat.col(0), mat.col(1));
  std::cout << str << std::endl;

  offset[0] += curr_tok->getMat().col(0).maxCoeff();
}

uint Sequencer::Step() {
  uint_fast8_t curr_char = ref[idx];
  std::cout << char(curr_char) << std::endl;
  if (curr_char == 32) {
    offset[0] += spacing;
  } else {
    std::string curr_tok_data = token_lut[curr_char];
    Token curr_tok(curr_tok_data);
    applyTransform(&curr_tok);
    std::cout << curr_tok.getGCode() << std::endl;
  }

  // uint_fast8_t curr_char = ref[idx];
  // char *buf;
  // buf = strtok(token_lut[curr_char].c_str());
  // struct gcom_t = {
  //
  // };

  return idx++;
}
