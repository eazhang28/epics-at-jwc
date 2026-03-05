#ifndef PARSE_HPP
#define PARSE_HPP

#include "Eigen/Core"
#include "Eigen/Dense"
#include <cstdint>
#include <string>
extern "C" {
#include "parser.h"
}

class Token {
private:
  Eigen::MatrixXd matrix;
  std::string gcode;

public:
  Eigen::MatrixXd gcode_to_matrix();
  std::string matrix_to_gcode(Eigen::MatrixXd colx, Eigen::MatrixXd coly);
  Token(std::string data);
  Eigen::MatrixXd operator+(float value[]);
  Eigen::MatrixXd operator*(float value);
  Eigen::MatrixXd getMat();
  std::string getGCode();
};

// struct gcom_t {
//   uint32_t x;
//   uint32_t y;
//   uint32_t idx : 10;
//   uint32_t chr : 8;
//   uint32_t line : 6;
//   uint32_t instr_s : 6;
//   uint32_t instr_p : 1; // G or M mode
//   uint32_t edit : 1;
// } __attribute__((packed));
//
// int *com_gylph(struct gcom_t com);
//
// #define BUF_LEN_MAX 256
#endif
