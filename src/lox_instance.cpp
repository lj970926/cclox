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
    auto func = std::dynamic_pointer_cast<LoxFunction>(method);
    bool is_initializer = (name.lexeme() == "init");
    return func->Bind(std::const_pointer_cast<LoxInstance>(shared_from_this()),
                      is_initializer);
  }

  throw RuntimeError(name, "Undefined property '" + name.lexeme() + "'.");
}

void LoxInstance::Set(Token name, OptionalLiteral value) {
  fields_[name.lexeme()] = value;
}

} //namespace cclox