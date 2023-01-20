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
class LoxCallable {
 public:
  virtual OptionalLiteral Call(Executor& executor, const std::vector<OptionalLiteral>& arguments) = 0;
  virtual size_t Arity() = 0;
};

class LoxFunction: public LoxCallable {
 public:
  LoxFunction(std::unique_ptr<FunctionStmt> declaration);
  OptionalLiteral Call(Executor& executor, const std::vector<OptionalLiteral>& arguments) override;
  size_t Arity() override;
 private:
  std::unique_ptr<FunctionStmt> declaration_;
};

} //namespace cclox

#endif  // CCLOX_CALLABLE_H
