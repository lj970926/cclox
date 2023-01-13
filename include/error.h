//
// Created by lijin on 2023/1/13.
//

#ifndef CCLOX_ERROR_H
#define CCLOX_ERROR_H
#include <stdexcept>
#include <string>

#include "token.h"

namespace cclox {
class RuntimeError: public std::runtime_error {
 public:
  RuntimeError(Token token, const std::string& message)
      : std::runtime_error(message.c_str()), token_(token) {}
  Token token() const { return token_; }
 private:
  Token token_;
};
} //namespace cclox

#endif  // CCLOX_ERROR_H
