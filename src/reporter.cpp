//
// Created by lijin on 23-1-6.
//

#include "reporter.h"

#include <iostream>

namespace cclox {
void ErrorReporter::set_error(int line, const std::string &message) {
  error_msgs_.emplace_back("[line " + std::to_string(line) + "] Error: " + message);
  status_ = LoxStatus::ERROR;
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