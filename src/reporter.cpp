//
// Created by lijin on 23-1-6.
//

#include "reporter.h"

#include <iostream>

namespace cclox {
void ErrorReporter::set_error(size_t line, const std::string &message) {
  error_msgs_.emplace_back("[line " + std::to_string(line) + "] Error: " + message);
  status_ = LoxStatus::ERROR;
}

void ErrorReporter::set_error(Token token, const std::string &message) {
  std::string error_msg;
  if (token.type() == TokenType::LOX_EOF) {
    error_msg = "at end, " + message;
  } else {
    error_msg = "at '" + token.lexeme() + "', " + message;
  }
  set_error(token.line(), error_msg);
}

LoxStatus ErrorReporter::status() const { return status_; }

void ErrorReporter::clear() {
  error_msgs_.clear();
  status_ = LoxStatus::OK;
}

void ErrorReporter::Print() const {
  for (const auto& s: error_msgs_) {
    std::cout << s << std::endl;
  }
}

}