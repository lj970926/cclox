//
// Created by lijin on 2023/1/4.
//

#include "interpreter.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include "scanner.h"

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
  Scanner scanner(source, reporter_);
  auto tokens = scanner.ScanTokens();
  for (const auto& token: tokens) {
    std::cout << static_cast<std::string>(token) << std::endl;
  }
}
} //namespace cclox