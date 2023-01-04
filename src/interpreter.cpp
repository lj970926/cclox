//
// Created by lijin on 2023/1/4.
//

#include "interpreter.h"

#include <fstream>
#include <sstream>
#include <iostream>

namespace cclox {
void Interpreter::RunFile(const std::string &path) {
  std::ifstream ifs(path);
  std::ostringstream oss;
  oss << ifs.rdbuf();
  Run(oss.str());
  if (had_error_)
    exit(65);
}

void Interpreter::RunPrompt() {
  std::string line;
  while (std::cout << ">" && std::getline(std::cin, line)) {
    Run(line);
    had_error_ = false;
  }
}

void Interpreter::Run(const std::string &source) {

}
} //namespace cclox