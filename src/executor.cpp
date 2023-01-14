//
// Created by lijin on 23-1-13.
//

#include "executor.h"

#include <variant>
#include <string>

#include "error.h"

namespace cclox {
Executor::Executor(ErrorReporter &reporter): reporter_(reporter) {}

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
      CheckNumberOperand(expr.token, {right});
      value_ = -DOUBLE_VALUE(right);
      break ;
    case TokenType::BANG:
      value_ = BOOL_STRING(!IsTruthy(right));
      break ;
    default:
      break ;
  }
}

void Executor::VisitBinary(const BinaryExpr &expr) {
  auto left = Evaluate(*expr.left.get());
  auto right = Evaluate(*expr.right.get());

  switch (expr.token.type()) {
    case TokenType::GREATER:
      CheckNumberOperand(expr.token, {left, right});
      value_ = BOOL_STRING(DOUBLE_VALUE(left) > DOUBLE_VALUE(right));
      break ;
    case TokenType::GREATER_EQUAL:
      CheckNumberOperand(expr.token, {left, right});
      value_ = BOOL_STRING(DOUBLE_VALUE(left) >= DOUBLE_VALUE(right));
      break ;
    case TokenType::LESS:
      CheckNumberOperand(expr.token, {left, right});
      value_ = BOOL_STRING(DOUBLE_VALUE(left) < DOUBLE_VALUE(right));
      break ;
    case TokenType::LESS_EQUAL:
      CheckNumberOperand(expr.token, {left, right});
      value_ = BOOL_STRING(DOUBLE_VALUE(left) <= DOUBLE_VALUE(right));
      break ;
    case TokenType::MINUS:
      CheckNumberOperand(expr.token, {left, right});
      value_ = DOUBLE_VALUE(left) - DOUBLE_VALUE(right);
      break ;
    case TokenType::SLASH:
      CheckNumberOperand(expr.token, {left, right});
      value_ = DOUBLE_VALUE(left) / DOUBLE_VALUE(right);
      break ;
    case TokenType::STAR:
      CheckNumberOperand(expr.token, {left, right});
      value_ = DOUBLE_VALUE(left) * DOUBLE_VALUE(right);
      break ;
    case TokenType::PLUS:
      if (IS_DOUBLE(left) && IS_DOUBLE(right)) {
        value_ = DOUBLE_VALUE(left) + DOUBLE_VALUE(right);
      } else if (IS_STRING(left) && IS_STRING(right)) {
        value_ = STRING_VALUE(left) + STRING_VALUE(right);
      } else {
        throw RuntimeError(expr.token, "Operand must be two nums or two strings.");
      }
      break ;
    case TokenType::EQUAL_EQUAL:
      value_ = BOOL_STRING(IsEqual(left, right));
      break ;
    case TokenType::BANG_EQUAL:
      value_ = BOOL_STRING(!IsEqual(left, right));
      break ;
    default:
      break ;
  }
}

OptionalLiteral Executor::Execute(const ExprPtr &expr) {
  try {
    return Evaluate(*expr.get());
  } catch (const RuntimeError& error) {
    reporter_.set_error(error.token().line(), error.what());
  }
  return {};
}

OptionalLiteral Executor::Evaluate(const Expr &expr) {
  expr.Accept(*this);
  return value_;
}

bool Executor::IsTruthy(OptionalLiteral value) const {
  if (IS_STRING(value)) {
    std::string str_val = STRING_VALUE(value);
    return !(str_val == "false" || str_val == "nil");
  }

  return true;
}

bool Executor::IsEqual(OptionalLiteral lhs, OptionalLiteral rhs) const {
  return lhs == rhs;
}

void Executor::CheckNumberOperand(Token op, const std::initializer_list<OptionalLiteral>& values) const {
  for (const auto& value: values) {
    if (!IS_DOUBLE(value))
      throw RuntimeError(op, "Operand must be a number.");
  }
}
} //namespace cclox