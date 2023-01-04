//
// Created by lijin on 2023/1/4.
//

#ifndef CCLOX_INTERPRETER_H
#define CCLOX_INTERPRETER_H
#include <string>

namespace cclox {
class Interpreter {
 public:
  void RunFile(const std::string& path);
  void RunPrompt();
  Interpreter(const Interpreter&) = delete;
  Interpreter& operator=(const Interpreter&) = delete;
 private:
  bool had_error_ = false;
};
} //namespace cclox

#endif//CCLOX_INTERPRETER_H
