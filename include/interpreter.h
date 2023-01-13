//
// Created by lijin on 2023/1/4.
//

#ifndef CCLOX_INTERPRETER_H
#define CCLOX_INTERPRETER_H
#include <string>

#include "reporter.h"

namespace cclox {
class Interpreter {
 public:
  void RunFile(const std::string& path);
  void RunPrompt();
  Interpreter(const Interpreter&) = delete;
  Interpreter& operator=(const Interpreter&) = delete;
  Interpreter() = default;
 private:
  bool had_error_ = false;
  bool had_runtime_error_ = false;
  ErrorReporter reporter_;

  void Run(const std::string& source);

};
} //namespace cclox

#endif//CCLOX_INTERPRETER_H
