//
// Created by lijin on 23-1-27.
//

#include "lox_class.h"

#include "lox_instance.h"

namespace cclox {
OptionalLiteral LoxClass::Call(Executor &executor, const std::vector<OptionalLiteral> &arguments) {
  InstancePtr instance = std::make_shared<LoxInstance>(std::dynamic_pointer_cast<LoxClass>(shared_from_this()));
  return  instance;
}

size_t LoxClass::Arity() {
  return 0;
}

CallablePtr LoxClass::FindMethod(const std::string &name) const {
  if (methods_.contains(name)) {
    return methods_.at(name);
  }
  return nullptr;
}
}