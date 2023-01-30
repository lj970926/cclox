//
// Created by lijin on 23-1-20.
//

#include "callable.h"

#include <optional>

#include "environment.h"
#include "return.h"
#include "common.h"

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
    executor.ExecuteBlock(declaration_->body, env);
  } catch (const Return& ret) {
    return ret.value();
  }

  return std::nullopt;
}

size_t LoxFunction::Arity() {
  return declaration_->params.size();
}

CallablePtr LoxFunction::Bind(InstancePtr instance, bool is_initializer) {
  return std::make_shared<LoxMethod>(shared_from_this(), instance, is_initializer);
}

LoxMethod::LoxMethod(CallablePtr callable, InstancePtr instance,
                     bool is_initializer): is_initializer_(is_initializer) {
  auto func = std::dynamic_pointer_cast<LoxFunction>(callable);
  if (func) {
    function_ = func;
    env_ = std::make_shared<Environment>(func->closure());
    env_->Define("this", instance);
  }
}

OptionalLiteral LoxMethod::Call(Executor &executor, const std::vector<OptionalLiteral> &arguments) {
  VariableRestorer restorer(function_->closure());
  function_->closure() = env_;
  auto res = function_->Call(executor, arguments);
  return is_initializer_ ? env_->GetAt(0, "this") : res;
}

} //namespace cclox