//
// Created by lijin on 23-1-16.
//

#include "environment.h"
#include "error.h"

namespace cclox {
void Environment::Define(const std::string &name, OptionalLiteral value) {
  variables_.emplace(name, value);
}

OptionalLiteral Environment::Get(Token name) {
  if (variables_.find(name.lexeme()) != variables_.end()) {
    return variables_[name.lexeme()];
  }
  throw RuntimeError(name, "Undefined variable '" + name.lexeme() + "'.");
}

} //namespace cclox