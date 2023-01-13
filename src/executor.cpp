//
// Created by lijin on 23-1-13.
//

#include "executor.h"

#include <variant>
#include <string>

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
    case TokenType::BANG:
      value_ = IsTruthy(right) ? "false" : "true";
      break ;
    default:
      break ;
  }
}

OptionalLiteral Executor::Evaluate(const Expr &expr) {
  expr.accept(*this);
  return value_;
}

bool Executor::IsTruthy(OptionalLiteral value) const {
  auto literal = value.value();
  if (literal.index() == 0) {
    std::string str_val = std::get<std::string>(literal);
    return !(str_val == "false" || str_val == "nil");
  }

  return true;
}
} //namespace cclox