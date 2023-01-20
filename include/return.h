//
// Created by lijin on 2023/1/20.
//

#ifndef CCLOX_RETURN_H
#define CCLOX_RETURN_H

#include <exception>

#include "token.h"

namespace cclox {
class Return: public std::exception {
 public:
  Return(OptionalLiteral value): value_(value) {}
  OptionalLiteral value() const { return value_; }
 private:
  OptionalLiteral value_;
};
} //namespace cclox

#endif  // CCLOX_RETURN_H
