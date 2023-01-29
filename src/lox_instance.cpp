//
// Created by lijin on 23-1-28.
//
#include "lox_instance.h"

#include "error.h"

namespace cclox {

OptionalLiteral LoxInstance::Get(Token name) const {
  if (fields_.contains(name.lexeme())) {
    return fields_.at(name.lexeme());
  }

  throw RuntimeError(name, "Undefined property '" + name.lexeme() + "'.");
}

void LoxInstance::Set(Token name, OptionalLiteral value) {
  fields_[name.lexeme()] = value;
}

} //namespace cclox