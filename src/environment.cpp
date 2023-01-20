//
// Created by lijin on 23-1-16.
//

#include "environment.h"
#include "error.h"

namespace cclox {

Environment::Environment(EnvironPtr enclosing): enclosing_(enclosing) {}

void Environment::Define(const std::string &name, OptionalLiteral value) {
  variables_.emplace(name, value);
}

OptionalLiteral Environment::Get(Token name) {
  if (variables_.find(name.lexeme()) != variables_.end()) {
    return variables_[name.lexeme()];
  }

  if (enclosing_) {
    return  enclosing_->Get(name);
  }

  throw RuntimeError(name, "Undefined variable '" + name.lexeme() + "'.");
}

void Environment::Assign(Token name, OptionalLiteral value) {
  if (variables_.find(name.lexeme()) != variables_.end()) {
    variables_[name.lexeme()] = value;
    return ;
  }

  if (enclosing_) {
    enclosing_->Assign(name, value);
    return ;
  }

  throw RuntimeError(name, "Undefined variable '" + name.lexeme() + "'.");
}

} //namespace cclox