//
// Created by lijin on 23-1-16.
//

#ifndef CCLOX_ENVIRONMENT_H
#define CCLOX_ENVIRONMENT_H

#include <unordered_map>
#include <string>

#include "common.h"
#include "token.h"

namespace cclox {
class Environment: public NonCopyable {
 public:
  void Define(const std::string& name, OptionalLiteral value);
  OptionalLiteral Get(Token name);
  void Assign(Token name, OptionalLiteral value);
 private:
  std::unordered_map<std::string, OptionalLiteral> variables_;
};
} //namespace cclox

#endif  // CCLOX_ENVIRONMENT_H
