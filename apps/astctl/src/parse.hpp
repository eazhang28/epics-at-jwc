#ifndef PARSE_HPP
#define PARSE_HPP

#include "Eigen/Core"
#include "Eigen/Dense"
#include "main.h"
#include <memory>
#include <string>
extern "C" {

#include "parser.h"
}

class Token {
private:
  Eigen::MatrixXd matrix;
  std::string gcode;
  void gcode_to_matrix(parser_t parser);

public:
  Token(std::string data);
  Eigen::MatrixXd getMat();
};

Eigen::MatrixXd gcode_to_matrix(parser_t parser, std::string data);

#endif
