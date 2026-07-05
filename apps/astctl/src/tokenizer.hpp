#ifndef PARSE_HPP
#define PARSE_HPP

#include "Eigen/Core"
#include "Eigen/Dense"
#include <string>

class Token {
private:
  Eigen::MatrixXd matrix;
  std::string gcode;

public:
  Eigen::MatrixXd gcode_to_matrix();
  char ch;
  std::string matrix_to_gcode(Eigen::MatrixXd colx, Eigen::MatrixXd coly);
  Token(char ch, std::string data);
  Eigen::MatrixXd operator+(float value[]);
  Eigen::MatrixXd operator*(float value);
  Eigen::MatrixXd getMat();
  std::string getGCode();
  void setGcode(std::string gc);

  float *getOffset();
  void modifyOffset(float amt, char mode);
};

#endif
