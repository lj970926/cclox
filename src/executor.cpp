//
// Created by lijin on 23-1-13.
//

#include "executor.h"

#include <variant>
#include <string>
#include <iostream>

#include "error.h"
#include "callable.h"
#include "return.h"
#include "lox_class.h"
#include "lox_instance.h"
#include "common.h"

namespace cclox {
Executor::Executor(ErrorReporter &reporter): reporter_(reporter), global_(new Environment()), environment_(global_) {}

void Executor::VisitLiteral(const LiteralExpr &expr) {
  value_ = expr.value;
}

void Executor::VisitGrouping(const GroupingExpr &expr) {
  EvaluateExpr(*expr.expr.get());
}

void Executor::VisitUnary(const UnaryExpr &expr) {
  auto right = EvaluateExpr(*expr.right.get());
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
  auto left = EvaluateExpr(*expr.left.get());
  auto right = EvaluateExpr(*expr.right.get());

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

void Executor::VisitVariable(const VariableExpr &expr) {
  value_ = LookUpVariable(expr.name, expr);
}

void Executor::VisitAssign(const AssignExpr &expr) {
  auto value = EvaluateExpr(*expr.expr);
  if (locals_.contains(&expr)) {
    size_t distance = locals_.at(&expr);
    environment_->AssignAt(distance, expr.name, value);
  } else {
    global_->Assign(expr.name, value);
  }
  value_ = value;
}

void Executor::VisitLogic(const cclox::LogicExpr &expr) {
  auto left = EvaluateExpr(*expr.left);

  if (expr.op.type() == TokenType::OR) {
    if (IsTruthy(left)) {
      value_ = left;
      return ;
    }
  } else {
    if (!IsTruthy(left)) {
      value_ = left;
      return ;
    }
  }

  value_ = EvaluateExpr(*expr.right);
}

void Executor::VisitCall(const CallExpr& expr) {
  auto callee = EvaluateExpr(*expr.callee);

  std::vector<OptionalLiteral> arguments;

  for (const auto& argument: expr.arguments) {
    arguments.emplace_back(EvaluateExpr(*argument));
  }

  if (!IS_CALLABLE(callee))
    throw RuntimeError(expr.paren, "Can only call functions and classes.");

  CallablePtr function = CALLABLE_VALUE(callee);
  if (expr.arguments.size() != function->Arity()) {
    throw RuntimeError(expr.paren, "Expected " +
                                       std::to_string(function->Arity()) +
                                       " arguments but got " +
                                       std::to_string(arguments.size()) + ".");
  }

  value_ = function->Call(*this, arguments);
}

void Executor::VisitGet(const GetExpr &expr) {
  OptionalLiteral value = EvaluateExpr(*expr.object);
  if (IS_INSTANCE(value)) {
    InstancePtr instance = INSTANCE_VALUE(value);
    value_ = instance->Get(expr.name);
    return ;
  }

  throw RuntimeError(expr.name, "Only instances have properties.");
}

void Executor::VisitSet(const SetExpr &expr) {
  OptionalLiteral object = EvaluateExpr(*expr.object);
  if (IS_INSTANCE(object)) {
    OptionalLiteral value = EvaluateExpr(*expr.value);
    InstancePtr instance = INSTANCE_VALUE(object);
    instance->Set(expr.name, value);
    value_ = value;
    return ;
  }
  throw RuntimeError(expr.name, "Only instances have fields.");
}

void Executor::VisitThis(const ThisExpr &expr) {
  value_ = LookUpVariable(expr.keyword, expr);
}

void Executor::VisitSuper(const SuperExpr &expr) {
  size_t distance = locals_.at(&expr);
  auto superclass = std::dynamic_pointer_cast<LoxClass>(
      CALLABLE_VALUE(environment_->GetAt(distance, "super"))
      );
  InstancePtr instance = INSTANCE_VALUE(environment_->GetAt(distance - 1, "this"));
  auto method = std::dynamic_pointer_cast<LoxFunction>(
      superclass->FindMethod(expr.method.lexeme())
      );
  if (!method) {
    throw RuntimeError(expr.method, "Undefined property '" + expr.method.lexeme() + "'.");
  }
  value_ = method->Bind(instance, expr.method.lexeme() == "init");
}

void Executor::VisitExpressionStmt(const ExpressionStmt &stmt) {
  EvaluateExpr(*stmt.expr);
}

void Executor::VisitPrintStmt(const PrintStmt &stmt) {
    auto value = EvaluateExpr(*stmt.expr.get());
    std::cout << GetLiteralStr(value) << std::endl;
}

void Executor::VisitVarStmt(const VarStmt &stmt) {
  OptionalLiteral value = std::nullopt;
  if (stmt.init_expr) {
    value = EvaluateExpr(*stmt.init_expr);
  }
  environment_->Define(stmt.name.lexeme(), value);
}

void Executor::VisitBlockStmt(const BlockStmt &stmt) {
  ExecuteBlock(stmt.stmts, std::make_shared<Environment>(environment_));
}

void Executor::VisitIfStmt(const cclox::IfStmt &stmt) {
  if (IsTruthy(EvaluateExpr(*stmt.condition))) {
    EvaluateStmt(*stmt.then_branch);
  } else if (stmt.else_branch) {
    EvaluateStmt(*stmt.else_branch);
  }
}

void Executor::VisitWhileStmt(const WhileStmt &stmt) {
  while (IsTruthy(EvaluateExpr(*stmt.condition))) {
    EvaluateStmt(*stmt.stmt);
  }
}

void Executor::VisitFunctionStmt(const FunctionStmt &stmt) {
  auto function_stmt = std::make_unique<FunctionStmt>(
      stmt.name,
      stmt.params,
      std::move(const_cast<std::vector<StmtPtr>&>(stmt.body)));

  CallablePtr function = std::make_shared<LoxFunction>(std::move(function_stmt), environment_);
  environment_->Define(stmt.name.lexeme(), function);
}

void Executor::VisitReturnStmt(const cclox::ReturnStmt &stmt) {
  OptionalLiteral value = std::nullopt;
  if (stmt.expr) {
    value = EvaluateExpr(*stmt.expr);
  }
  throw Return(value);
}

void Executor::VisitClassStmt(const ClassStmt &stmt) {
  OptionalLiteral superclass = std::nullopt;
  if (stmt.superclass) {
    superclass = EvaluateExpr(*stmt.superclass);
    if (!IS_CALLABLE(superclass) ||
        std::dynamic_pointer_cast<LoxClass>(CALLABLE_VALUE(superclass)) == nullptr) {
      throw RuntimeError(
          dynamic_cast<const VariableExpr*>(stmt.superclass.get())->name,
          "Superclass must be a class");
    }
  }
  environment_->Define(stmt.name.lexeme(), std::nullopt);
  VariableRestorer restorer(environment_);
  if (stmt.superclass) {
    environment_ = std::make_shared<Environment>(environment_);
    environment_->Define("super", superclass);
  }

  std::unordered_map<std::string, CallablePtr> methods;
  for (const auto& method: stmt.methods) {
    auto func_stmt = dynamic_cast<const FunctionStmt*>(method.get());
    auto declaration = std::make_unique<FunctionStmt>(
        func_stmt->name,
        func_stmt->params,
        std::move(const_cast<FunctionStmt*>(func_stmt)->body));
    CallablePtr function = std::make_shared<LoxFunction>(std::move(declaration), environment_);
    methods[func_stmt->name.lexeme()] = function;
  }
  CallablePtr lox_class;
  if (superclass) {
    lox_class = std::make_shared<LoxClass>(stmt.name,
                                           std::dynamic_pointer_cast<LoxClass>(CALLABLE_VALUE(superclass)),
                                               methods);
  } else {
    lox_class = std::make_shared<LoxClass>(stmt.name, nullptr, methods);
  }

  environment_->Assign(stmt.name, lox_class);
}

void Executor::Execute(const std::vector<StmtPtr>& stmts) {
  try {
    for (const auto& stmt: stmts) {
      EvaluateStmt(*stmt);
    }
  } catch (const RuntimeError& error) {
    reporter_.set_error(error.token().line(), error.what());
  }
}

OptionalLiteral Executor::Execute(const ExprPtr &expr) {
  try {
    return EvaluateExpr(*expr);
  } catch (const RuntimeError& error) {
    reporter_.set_error(error.token().line(), error.what());
  }
  return {};
}

void Executor::ExecuteBlock(const std::vector<StmtPtr> &stmts, EnvironPtr environment) {
  VariableRestorer restorer(environment_);
  environment_ = environment;
  Execute(stmts);
}

void Executor::EvaluateStmt(const Stmt& stmt) {
  stmt.Accept(*this);
}

OptionalLiteral Executor::EvaluateExpr(const Expr &expr) {
  expr.Accept(*this);
  return value_;
}

bool Executor::IsTruthy(OptionalLiteral value) const {
  if (value == std::nullopt)
    return false;
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

void Executor::Resolve(const Expr *expr, size_t depth) {
  locals_.emplace(expr, depth);
}

OptionalLiteral Executor::LookUpVariable(Token name, const Expr &expr) {
  if (locals_.contains(&expr)) {
    size_t distance = locals_.at(&expr);
    return environment_->GetAt(distance, name.lexeme());
  }

  return global_->Get(name);
}
} //namespace cclox