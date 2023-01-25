//
// Created by lijin on 2023/1/4.
//

#include "interpreter.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include "common.h"
#include "scanner.h"
#include "parser.h"
#include "executor.h"
#include "resolver.h"

namespace cclox {
void Interpreter::RunFile(const std::string &path) {
  std::ifstream ifs(path);
  std::ostringstream oss;
  oss << ifs.rdbuf();
  Run(oss.str());
  if (had_error_)
    exit(65);

  if (had_runtime_error_)
    exit(70);
}

void Interpreter::RunPrompt() {
  std::string line;
  while (std::cout << ">" && std::getline(std::cin, line)) {
    Run(line);
    had_error_ = false;
    had_runtime_error_ = false;
    reporter_.clear();
  }
  std::cout << "\n\nBye!\n";
}

void Interpreter::Run(const std::string &source) {
  Scanner scanner(source, reporter_);
  auto tokens = scanner.ScanTokens();

  if (reporter_.status() != LoxStatus::OK) {
    reporter_.Print();
    had_error_ = true;
    return ;
  }

  Parser parser(tokens, reporter_);
  auto statements = parser.Parse();

  if (reporter_.status() != LoxStatus::OK) {
    reporter_.Print();
    had_error_ = true;
    return ;
  }

  Resolver resolver(executor_, reporter_);
  resolver.Resolve(statements);

  executor_ .Execute(statements);

  if (reporter_.status() != LoxStatus::OK) {
    reporter_.Print();
    had_runtime_error_ = true;
    return ;
  }
}
} //namespace cclox