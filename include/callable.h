//
// Created by lijin on 23-1-19.
//

#ifndef CCLOX_CALLABLE_H
#define CCLOX_CALLABLE_H

#include <vector>
#include <memory>

#include "token.h"
#include "executor.h"
#include "stmt.h"

namespace cclox {
class LoxCallable: public std::enable_shared_from_this<LoxCallable>{
 public:
  virtual OptionalLiteral Call(Executor& executor, const std::vector<OptionalLiteral>& arguments) = 0;
  virtual size_t Arity() = 0;
};

class LoxFunction: public LoxCallable {
 public:
  LoxFunction(std::unique_ptr<FunctionStmt> declaration, EnvironPtr closure);
  OptionalLiteral Call(Executor& executor, const std::vector<OptionalLiteral>& arguments) override;
  size_t Arity() override;
  CallablePtr Bind(InstancePtr instance);
  EnvironPtr& closure() { return closure_; }
 private:
  std::unique_ptr<FunctionStmt> declaration_;
  EnvironPtr closure_;
};

class LoxMethod: public LoxCallable {
 public:
  LoxMethod(CallablePtr func, InstancePtr instance);
  OptionalLiteral Call(Executor& executor, const std::vector<OptionalLiteral>& arguments)  override;
  size_t Arity() override { return function_->Arity(); }
 private:
  std::shared_ptr<LoxFunction> function_;
  EnvironPtr env_;
};

} //namespace cclox

#endif  // CCLOX_CALLABLE_H
