//
// Created by lijin on 23-1-27.
//

#include "lox_class.h"

#include "lox_instance.h"

namespace cclox {
OptionalLiteral LoxClass::Call(Executor &executor, const std::vector<OptionalLiteral> &arguments) {
  InstancePtr instance = std::make_shared<LoxInstance>(shared_from_this());
  return  instance;
}

size_t LoxClass::Arity() {
  return 0;
}
}