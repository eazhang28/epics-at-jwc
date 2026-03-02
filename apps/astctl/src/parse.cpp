#include "parse.hpp"
#include "Eigen/Dense"
#include "parser.h"
#include <memory>
#include <string>

// Eigen::MatrixXd gcode_to_matrix(parser_t parser, std::string data) {
//   gcode_points_t *point = parser.gcode;
//   std::vector<float> vector;
//
//   int dim = 0;
//   gcode_points_t *temp = parser.gcode;
//   do {
//     dim++;
//   } while ((temp = (gcode_points_t *)temp->next) != NULL);
//
//   int i = 0;
//   Eigen::MatrixXd m(dim, 2);
//   do {
//     m(i, 0) = point->ideal.x;
//     m(i, 1) = point->ideal.y;
//     i++;
//   } while ((point = (gcode_points_t *)point->next) != NULL);
//
//   return m;
// }

Token::Token(std::string data) {
  this->gcode = data;
  parser_t parser;
  parser_init(&parser);
  parser_read_gcode_text(&parser, this->gcode.c_str());
  gcode_to_matrix(parser);
  parser_free(&parser);
}

void Token::gcode_to_matrix(parser_t parser) {
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

  matrix = m;
}

Eigen::MatrixXd Token::getMat() { return matrix; }
