//
// Created by lijin on 23-1-28.
//
#include "lox_instance.h"

#include "error.h"
#include "lox_class.h"

namespace cclox {

OptionalLiteral LoxInstance::Get(Token name) const {
  if (fields_.contains(name.lexeme())) {
    return fields_.at(name.lexeme());
  }

  auto method = lox_class_->FindMethod(name.lexeme());
  if (method) {
    return method;
  }

  throw RuntimeError(name, "Undefined property '" + name.lexeme() + "'.");
}

void LoxInstance::Set(Token name, OptionalLiteral value) {
  fields_[name.lexeme()] = value;
}

} //namespace cclox