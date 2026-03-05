#include "tokenizer.hpp"
#include "Eigen/Dense"
#include "parser.h"
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

Token::Token(std::string data) {
  this->gcode = data;
  this->matrix = gcode_to_matrix();
}

Eigen::MatrixXd Token::gcode_to_matrix() {
  parser_t parser;
  parser_init(&parser);
  parser_read_gcode_text(&parser, gcode.c_str());
  gcode_points_t *point = parser.gcode;
  std::vector<float> vector;

  int dim = 0;
  gcode_points_t *temp = parser.gcode;
  do {
    dim++;
  } while ((temp = (gcode_points_t *)temp->next) != NULL);

  int i = 0;
  Eigen::MatrixXd m(dim, 2);
  do {
    m(i, 0) = point->ideal.x;
    m(i, 1) = point->ideal.y;
    i++;
  } while ((point = (gcode_points_t *)point->next) != NULL);

  parser_free(&parser);
  return m;
}

std::string Token::matrix_to_gcode(Eigen::MatrixXd colx, Eigen::MatrixXd coly) {
  std::istringstream iss(gcode);
  std::ostringstream oss;
  std::string line;
  uint idx = 0;
  float t1, t2 = 0;

  while (std::getline(iss, line)) {
    float x, y;
    t1 = colx(idx);
    t2 = coly(idx);
    if (std::sscanf(line.c_str(), TMPL2_G0, &x, &y) == 2) {
      // std::cout << x << y << std::endl;
      idx++;
      oss << "G0" << std::fixed << std::setprecision(2) << " X" << t1 << " Y"
          << t2;
    } else if (std::sscanf(line.c_str(), TMPL2_G01, &x, &y) == 2) {
      // std::cout << x << y << std::endl;
      idx++;
      oss << "G1" << std::fixed << std::setprecision(2) << " X" << t1 << " Y"
          << t2;
    } else {
      oss << line;
    }

    if (!iss.eof())
      oss << "\n";
  }
  return oss.str();
}

Eigen::MatrixXd Token::getMat() { return matrix; }
std::string Token::getGCode() { return gcode; }

Eigen::MatrixXd Token::operator+(float value[]) {
  Eigen::MatrixXd mat = this->matrix;
  mat = mat.array() + value[0];
  mat = mat.array() + value[1];
  return mat;
}

Eigen::MatrixXd Token::operator*(float value) { return this->matrix * value; }

// int com_glyph(struct gcom_t com, char *glyph) {
//   int ret = 1;
//   if (!com.instr_p && com.edit) {
//     snprintf(glyph, BUF_LEN_MAX, "G%d X%f Y%f\n", int(com.instr_s),
//              float(com.x), float(com.y));
//   }
//   return ret;
// }
