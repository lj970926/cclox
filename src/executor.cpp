//
// Created by lijin on 23-1-13.
//

#include "executor.h"

#include <variant>

namespace cclox {
void Executor::VisitLiteral(const LiteralExpr &expr) {
  value_ = expr.value;
}

void Executor::VisitGrouping(const GroupingExpr &expr) {
  Evaluate(*expr.expr.get());
}

void Executor::VisitUnary(const UnaryExpr &expr) {
  auto right = Evaluate(*expr.right.get());
  switch (expr.token.type()) {
    case TokenType::MINUS:
      value_ = - std::get<double>(right.value());
      break ;
    default:
      break ;
  }
}

OptionalLiteral Executor::Evaluate(const Expr &expr) {
  expr.accept(*this);
  return value_;
}
} //namespace cclox