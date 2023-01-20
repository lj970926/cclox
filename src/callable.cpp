//
// Created by lijin on 23-1-20.
//

#include "callable.h"

#include <optional>

#include "environment.h"
#include "return.h"

namespace cclox {
LoxFunction::LoxFunction(std::unique_ptr<FunctionStmt> declaration,
                         EnvironPtr closure)
    : declaration_(std::move(declaration)), closure_(closure){}

OptionalLiteral LoxFunction::Call(Executor &executor, const std::vector<OptionalLiteral> &arguments) {
  EnvironPtr env = std::make_shared<Environment>(closure_);

  for (int i = 0; i < declaration_->params.size(); ++i) {
    env->Define(declaration_->params[i].lexeme(), arguments[i]);
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