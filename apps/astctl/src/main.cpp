#ifndef MAIN
#define MAIN
#include "main.hpp"
#include "sequencer.hpp"
#include "sqlut.hpp"
#include "tokenizer.hpp"

extern "C" {
#include <sqlite3.h>
}
#include "Eigen/Dense"
#include <iostream>
#include <math.h>
#include <string>

int main(int argc, char *argv[]) {

  // this program is ran from the daemon state machine
  // receives input ./runtime_engine <string_input> <config file (in root)>
  // tasks, zero the machine, and then read for bounds
  // read config file, apply transformations as necessary
  // put input into sequencer
  // sequencer: retrieve necessary characters from sqlite LUT
  // sequence characters to match input order (delete from cache when no longer
  // needed) stream send gcode orders confirm end

  if (argc == 2) {
    std::string input = argv[1];
    Sequencer seq(input, SQLUT(input).getMap(), 5, 6, 7, 8);
    // std::map<int, std::string> res1 = seq.getLUT();
    //
    // for (const auto &pair : res1) {
    //   std::cout << char(pair.first) << '\n' << pair.second << std::endl;
    // }
    // for (const auto &pair : res1) {
    //   Token tkn(pair.second);
    //   std::cout << tkn.getMat() << std::endl;
    // }
    for (const auto &c : input) {
      std::cout << seq.Step() << std::endl;
    }
  }
  return 0;
}
#endif
