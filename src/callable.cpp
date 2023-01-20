//
// Created by lijin on 23-1-20.
//

#include "callable.h"

#include <optional>

namespace cclox {
LoxFunction::LoxFunction(const FunctionStmt &declaration): declaration_(declaration){}

OptionalLiteral LoxFunction::Call(Executor &executor, const std::vector<OptionalLiteral> &arguments) {
  return std::nullopt;
}

size_t LoxFunction::Arity() {
  return declaration_.params.size();
}
} //namespace cclox