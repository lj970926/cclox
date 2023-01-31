//
// Created by lijin on 23-1-27.
//

#include "lox_class.h"

#include "lox_instance.h"

namespace cclox {
OptionalLiteral LoxClass::Call(Executor &executor, const std::vector<OptionalLiteral> &arguments) {
  InstancePtr instance = std::make_shared<LoxInstance>(std::dynamic_pointer_cast<LoxClass>(shared_from_this()));
  auto initializer = std::dynamic_pointer_cast<LoxFunction>(FindMethod("init"));
  if (initializer) {
    initializer->Bind(instance, true)->Call(executor, arguments);
  }
  return  instance;
}

size_t LoxClass::Arity() {
  auto initializer = FindMethod("init");
  return initializer ? initializer->Arity() : 0;
}

CallablePtr LoxClass::FindMethod(const std::string &name) const {
  if (methods_.contains(name)) {
    return methods_.at(name);
  }
  if (superclass_) {
    return superclass_->FindMethod(name);
  }
  return nullptr;
}
}