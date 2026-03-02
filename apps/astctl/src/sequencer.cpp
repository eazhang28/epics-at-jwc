#include "sequencer.hpp"
#include "sqlut.hpp"

#include <map>
#include <string>
// sequencer will track offset, newline, whitespace, font, token cache, and
// input string
//
//

Sequencer::Sequencer(const std::string &input,
                     std::map<int, std::string> tokenMap) {
  initTokenMap(tokenMap);
}

void Sequencer::initTokenMap(std::map<int, std::string> tokenMap) {
  token_lut.clear();
  token_lut = tokenMap;
}

std::map<int, std::string> Sequencer::getLUT() const { return token_lut; }

uint Sequencer::Step() { return idx++; }
// look up
