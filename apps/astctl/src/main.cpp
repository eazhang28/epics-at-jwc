/**
 * @file main.cpp
 * @brief G-code translation and sequencing for named pipe input.
 *
 * Developed for Jackson Center for Conductive Education
 * Copyright (C) 2024 Jackson Center for Conductive Education
 * Licensed under the GNU General Public License v3.0 (GPL-3.0-or-later).
 *
 * This program runs as a systemd service. It reads input from a named pipe,
 * transforms it into machine-specific G-code using an SQLite-based LUT,
 * and handles machine safety bounds and state management.
 *
 * @author eazhang28
 * @version 1.0.0
 * @date 2026-04-28
 */

/* SPDX-License-Identifier: GPL-3.0-or-later */

#ifndef MAIN_CPP // Usually unique to the filename
#define MAIN_CPP

#include "main.hpp"
#include "Eigen/Dense"
#include <INIReader.h>
#include <chrono>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
extern "C" {
#include <sqlite3.h>
}

/**
 * @brief Tracks the current operational status of the machine.
 */
State currentState = IDLE;

/**
 * @brief Entry point for the pipe listener and G-code sequencer.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return int Execution status code.
 */
int main(int argc, char *argv[]) {
  std::string receive;
  std::ifstream fifo(
      "/tmp/input_pipe"); // Open named pipe for reading from frontend
  freopen("/tmp/output_pipe", "w", stdout); // Redirect stdout to output pipe
  std::ofstream logFile("./jwc_seq.log", std::ios::app);

  INIReader reader("./jwc_seq_config.ini");
  float HOFFSET = reader.GetReal("SEQUENCERCONFIG", "VOFFSET", 1);
  float VOFFSET = reader.GetReal("SEQUENCERCONFIG", "HOFFSET", 1);
  float SCALE = reader.GetReal("SEQUENCERCONFIG", "SCALE", 1);
  float SPACING = reader.GetReal("SEQUENCERCONFIG", "SPACING", 2);
  float MAXHLEN = reader.GetReal("SEQUENCERCONFIG", "MAXHLEN", 65);
  float MAXVLEN = reader.GetReal("SEQUENCERCONFIG", "MAXVLEN", 80);
  float ROTANGLE = reader.GetReal("SEQUENCERCONFIG", "ROTANGLE", 90);

  logFile << "HOFFSET: " << HOFFSET << ", VOFFSET: " << VOFFSET
          << ", SCALE: " << SCALE << ", SPACING: " << SPACING
          << ", MAXHLEN: " << MAXHLEN << ", MAXVLEN: " << MAXVLEN
          << ", ROTANGLE: " << ROTANGLE << std::endl;

  while (true) {
    switch (currentState) {
    case State::IDLE: {
      if (std::getline(fifo, receive)) {
        currentState = State::PROCESSING;
      } else if (fifo.eof()) {
        fifo.clear(); // Reset EOF to allow further reading from pipe
      }
    } break;

    case State::PROCESSING: {
      // Initialize sequencer with LUT data and coordinate bounds
      Sequencer seq(receive, SQLUT(receive).getMap(), HOFFSET, VOFFSET, SCALE,
                    SPACING, MAXHLEN, MAXVLEN, ROTANGLE);

      std::chrono::steady_clock::time_point start =
          std::chrono::steady_clock::now();
      for (const auto &c : receive) {
        // Step then check if bounds has exceeded.
        if (!seq.Step()) {
          std::cout << "M5" << std::endl; // Safety stop: bounds exceeded
          logFile << "Current Input Has Exceeded Bounds! " << std::endl;
          break;
        }
        // After a step is processed, check if we've exceeded the stall
        // threshold
        if ((std::chrono::steady_clock::now() - start).count() > 3) {
          logFile << "Exceeded Stall Threshold! Emergency Stop Engaged."
                  << std::endl;
          currentState = State::EMERGENCY_STOP;
          continue;
        }
        if (!seq.send_buffer.empty()) {
          std::cout << seq.send_buffer.front().getGCode() << std::endl;
          seq.send_buffer.pop();
          std::cout << "M5" << std::endl; // Stop spindle/tool between steps
        }
      }
      std::chrono::steady_clock::time_point end =
          std::chrono::steady_clock::now();
      std::chrono::duration<double> elapsed_seconds = end - start;
      logFile << "Processing Time: " << elapsed_seconds.count()
              << "s for input: " << receive << std::endl;
      std::cout << "G28" << std::endl; // Auto-home after processing
      currentState = State::IDLE;
      break;
    }
    case State::EMERGENCY_STOP: {
      // TODO: Monitor GPIO Pin XX for reset signal
      // TODO: Send kill signal to serial script
      return 0;
    }
    }
  }
  return 0;
}
#endif
