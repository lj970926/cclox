//
// Created by lijin on 23-1-13.
//

#ifndef CCLOX_EXECUTOR_H
#define CCLOX_EXECUTOR_H
#include <initializer_list>

#include "expr.h"
#include "token.h"
#include "reporter.h"

namespace cclox {
class Executor: public Visitor {
 public:
  Executor(ErrorReporter& reporter);
  void VisitBinary(const BinaryExpr& expr) override;
  void VisitGrouping(const GroupingExpr& expr) override;
  void VisitLiteral(const LiteralExpr& expr) override;
  void VisitUnary(const UnaryExpr& expr) override;
  OptionalLiteral Execute(const ExprPtr& expr);

 private:
  OptionalLiteral value_;
  ErrorReporter& reporter_;

  OptionalLiteral Evaluate(const Expr& expr);
  bool IsTruthy(OptionalLiteral value) const;
  bool IsEqual(OptionalLiteral lhs, OptionalLiteral rhs) const;
  void CheckNumberOperand(Token op, const std::initializer_list<OptionalLiteral>& values) const;
};
} //namespace cclox

#endif  // CCLOX_EXECUTOR_H
