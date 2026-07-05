#include "sequencer.hpp"
#include <cstdint>
#include <map>
#include <queue>
#include <string>
#include <tokenizer.hpp>

Sequencer::Sequencer(const std::string &input,
                     std::map<int, std::string> tokenMap, float h_off,
                     float v_off, float scale, float spacing, float max_hlen,
                     float max_vlen, float rot_angle) {
  this->token_lut = tokenMap;
  this->offset[0] = v_off, this->offset[1] = h_off;
  this->h_off = h_off, this->v_off = v_off;
  this->scale = scale;
  this->spacing = spacing;
  this->max_hlen = max_hlen;
  this->max_vlen = max_vlen;
  this->rot_angle = rot_angle;
  this->rot = Eigen::Rotation2Dd(rot_angle * M_PI / 180).toRotationMatrix();
  this->ref = input;
  this->space_flag = 0;
}

int Sequencer::applyTransform(Token *curr_tok) {
  Eigen::MatrixXd mat = curr_tok->getMat();
  // mat = mat * scale;
  mat = mat *
        rot.transpose(); // note: current set of gcode characters is not rotated
                         // for the machine, so we need to rotate the matrix
                         // to match the machine's coordinate system
  float left_edge = mat.col(1).array().minCoeff();
  float width = mat.col(1).maxCoeff() - left_edge;
  mat.col(1).array() -= left_edge; // shift the character to the leftmost
                                   // position to make it easier to apply
                                   // horizontal offset and spacing
  float next_xoffset = offset[1];
  float next_yoffset = offset[0];
  if (next_yoffset + width >=
      max_hlen -
          h_off) { // if current token has a coordinate that exceeds the max
                   // horizontal length, we need to move down to the next line
                   // and reset the horizontal offset and reset the gcode of
                   // the current token to match the new coordinates. We also
                   // need to check if the vertical offset exceeds the max
                   // vertical length, in which case we need to move back to
                   // the top and reset the horizontal offset as well.
    float char_height = mat.col(0).maxCoeff() - mat.col(0).minCoeff();
    next_yoffset = h_off;
    next_xoffset += char_height + (spacing * space_flag);
  }
  if (next_xoffset + mat.col(0).maxCoeff() >= max_vlen - v_off) {
    return 0;
  }
  offset[0] = next_yoffset;
  offset[1] = next_xoffset;
  mat.col(0).array() += offset[1];
  mat.col(1).array() += offset[0];
  offset[0] += width + (spacing * space_flag);
  curr_tok->setGcode(curr_tok->matrix_to_gcode(mat.col(0), mat.col(1)));
  return 1;
}

uint Sequencer::Step() {
  uint_fast8_t curr_char = ref[idx];
  std::string curr_tok_data;
  space_flag = 0;
  if (idx > 0 && curr_char == 32) {
    space_flag = 1;
    curr_tok_data = "G0 X" + std::to_string(0) + " Y" + std::to_string(0);
  } else {
    curr_tok_data = token_lut[curr_char];
  }
  Token curr_tok(char(curr_char), curr_tok_data);
  if (!applyTransform(&curr_tok))
    return 0;
  send_buffer.push(curr_tok);
  idx++;
  return 1;
}
