//
// Created by lijin on 23-1-16.
//

#ifndef CCLOX_ENVIRONMENT_H
#define CCLOX_ENVIRONMENT_H

#include <unordered_map>
#include <string>
#include <memory>

#include "common.h"
#include "token.h"

namespace cclox {

class Environment;
using EnvironPtr = std::shared_ptr<Environment>;

class Environment: public NonCopyable {
 public:

  Environment() = default;
  explicit Environment(std::shared_ptr<Environment> enclosing);

  void Define(const std::string& name, OptionalLiteral value);
  OptionalLiteral Get(Token name);
  OptionalLiteral GetAt(size_t distance, const std::string& name);
  void Assign(Token name, OptionalLiteral value);
  void AssignAt(size_t distance, Token name, OptionalLiteral value);

 private:
  std::unordered_map<std::string, OptionalLiteral> variables_;
  EnvironPtr enclosing_;

  EnvironPtr Ancestor(size_t distance);
};
} //namespace cclox

#endif  // CCLOX_ENVIRONMENT_H
