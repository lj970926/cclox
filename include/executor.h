//
// Created by lijin on 23-1-13.
//

#ifndef CCLOX_EXECUTOR_H
#define CCLOX_EXECUTOR_H
#include "expr.h"
#include "token.h"

namespace cclox {
class Executor: public Visitor {
 public:
  void VisitBinary(const BinaryExpr& expr) override;
  void VisitGrouping(const GroupingExpr& expr) override;
  void VisitLiteral(const LiteralExpr& expr) override;
  void VisitUnary(const UnaryExpr& expr) override;
 private:
  OptionalLiteral value_;

  OptionalLiteral Evaluate(const Expr& expr);
  bool IsTruthy(OptionalLiteral value) const;
};
} //namespace cclox

#endif  // CCLOX_EXECUTOR_H
