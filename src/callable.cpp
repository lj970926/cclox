//
// Created by lijin on 23-1-20.
//

#include "callable.h"

#include <optional>

#include "environment.h"
#include "return.h"

namespace cclox {
LoxFunction::LoxFunction(std::unique_ptr<FunctionStmt> declaration): declaration_(std::move(declaration)){}

OptionalLiteral LoxFunction::Call(Executor &executor, const std::vector<OptionalLiteral> &arguments) {
  EnvironPtr environ = std::make_shared<Environment>(executor.global_environment());

  for (int i = 0; i < declaration_->params.size(); ++i) {
    environ->Define(declaration_->params[i].lexeme(), arguments[i]);
  }

  try {
    executor.ExecuteBlock(declaration_->body, environ);
  } catch (const Return& ret) {
    return ret.value();
  }

  return std::nullopt;
}

size_t LoxFunction::Arity() {
  return declaration_->params.size();
}
} //namespace cclox