#ifndef MAIN
#define MAIN
#include "main.hpp"
#include "Eigen/Dense"
#include <fstream>
#include <iostream>
#include <string>
extern "C" {
#include <sqlite3.h>
}

State currentState = IDLE;

int main(int argc, char *argv[]) {
  // this program is ran from the daemon state machine
  // receives input ./runtime_engine <string_input> <config file (in root)>
  // tasks, zero the machine, and then read for bounds
  // read config file, apply transformations as necessary
  // put input into sequencer
  // sequencer: retrieve necessary characters from sqlite LUT
  // sequence characters to match input order (delete from cache when no longer
  // needed) stream send gcode orders confirm endl

  std::string old;
  std::string receive;
  std::ifstream fifo("/tmp/input_pipe");
  while (1) {
    switch (currentState) {
    case State::IDLE: {
      if (std::getline(fifo, receive)) {
        currentState = State::PROCESSING;
      } else if (fifo.eof()) {
        fifo.clear();
      }
    } break;
    case State::PROCESSING: {
      Sequencer seq(receive, SQLUT(receive).getMap(), 1, 0, 1, 2, 65, 80, 90);
      for (const auto &c : receive) {
        if (!seq.Step()) {
          std::cout << "M5" << std::endl; // stop command if the input exceeds
                                          // the bounds of the machine
          break;
        }
        if (!seq.send_buffer.empty()) {
          std::cout << seq.send_buffer.front().getGCode() << std::endl;
          seq.send_buffer.pop();
          std::cout << "M5" << std::endl;
        }
      }
      std::cout << "G28" << std::endl; // return machine to home command after
                                       // full input is processed.
      currentState = State::IDLE;
      break;
    }
    case State::EMERGENCY_STOP: {
      // ALWAYS make sure anytime the program needs to end, it reroutes here.
      // perform cleanup routines
      // read GPIO pin XX for reset signal, if reset signal is received, return
      // to idle state, and send a kill signal for serial script to stop sending
      // gcode commands to the machine.
      return 0;
      break;
    }
    }
  }

  return 0;
}
#endif
