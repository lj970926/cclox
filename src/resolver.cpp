//
// Created by lijin on 23-1-24.
//

#include "resolver.h"

#include "common.h"

namespace cclox {
void Resolver::VisitBlockStmt(const BlockStmt &stmt) {
  BeginScope();
  Resolve(stmt.stmts);
  EndScope();
}

void Resolver::VisitVarStmt(const VarStmt &stmt) {
  Declare(stmt.name);
  if (stmt.init_expr) {
    Resolve(*stmt.init_expr);
  }
  Define(stmt.name);
}

void Resolver::VisitFunctionStmt(const FunctionStmt &stmt) {
  Declare(stmt.name);
  Define(stmt.name);
  ResolveFunction(stmt, FunctionType::FUNCTION);
}

void Resolver::VisitExpressionStmt(const ExpressionStmt &stmt) {
  Resolve(*stmt.expr);
}

void Resolver::VisitIfStmt(const IfStmt &stmt) {
  Resolve(*stmt.condition);
  Resolve(*stmt.then_branch);
  if (stmt.else_branch)
    Resolve(*stmt.else_branch);
}

void Resolver::VisitPrintStmt(const PrintStmt &stmt) {
  Resolve(*stmt.expr);
}

void Resolver::VisitReturnStmt(const ReturnStmt &stmt) {
  if (current_function_ == FunctionType::NONE) {
    reporter_.set_error(stmt.keyword, "Can't return from top-level code.");
  }
  if (stmt.expr) {
    if (current_function_ == FunctionType::INITIALIZER) {
      reporter_.set_error(stmt.keyword, "Can't return a value from an initializer.");
    }
    Resolve(*stmt.expr);
  }
}

void Resolver::VisitWhileStmt(const WhileStmt &stmt) {
  Resolve(*stmt.condition);
  Resolve(*stmt.stmt);
}

void Resolver::VisitClassStmt(const ClassStmt &stmt) {
  VariableRestorer restorer(current_class_);
  current_class_ = ClassType::CLASS;
  Declare(stmt.name);
  Define(stmt.name);

  BeginScope();
  scopes_.back()["this"] = true;

  for (const auto& method: stmt.methods) {
    auto func_stmt = dynamic_cast<const FunctionStmt*>(method.get());
    FunctionType decl = FunctionType::METHOD;
    if (func_stmt->name.lexeme() == "init") {
      decl = FunctionType::INITIALIZER;
    }
    ResolveFunction(*func_stmt, decl);
  }
  EndScope();
}

void Resolver::VisitVariable(const VariableExpr &expr) {
  if (InInitializer(expr.name.lexeme())) {
    reporter_.set_error(expr.name, "Can't read local variable in its own initializer.");
  }

  ResolveLocal(expr, expr.name);
}

void Resolver::VisitBinary(const BinaryExpr &expr) {
  Resolve(*expr.left);
  Resolve(*expr.right);
}

void Resolver::VisitCall(const CallExpr &expr) {
  Resolve(*expr.callee);
  for (const auto& argument: expr.arguments) {
    Resolve(*argument);
  }
}

void Resolver::VisitGrouping(const GroupingExpr &expr) {
  Resolve(*expr.expr);
}

void Resolver::VisitLiteral(const LiteralExpr &expr) {}

void Resolver::VisitLogic(const LogicExpr &expr) {
  Resolve(*expr.left);
  Resolve(*expr.right);
}

void Resolver::VisitUnary(const UnaryExpr &expr) {
  Resolve(*expr.right);
}

void Resolver::VisitAssign(const AssignExpr &expr) {
  Resolve(*expr.expr);
  ResolveLocal(expr, expr.name);
}

void Resolver::VisitGet(const GetExpr &expr) {
  Resolve(* expr.object);
}

void Resolver::VisitSet(const SetExpr &expr) {
  Resolve(*expr.value);
  Resolve(*expr.object);
}

void Resolver::VisitThis(const ThisExpr &expr) {
  if (current_class_ == ClassType::NONE) {
    reporter_.set_error(expr.keyword, "Can't use 'this' outside of a class.");
    return ;
  }
  ResolveLocal(expr, expr.keyword);
}

void Resolver::Resolve(const Stmt &stmt) {
  stmt.Accept(*this);
}

void Resolver::Resolve(const Expr &expr) {
  expr.Accept(*this);
}

void Resolver::Resolve(const std::vector<StmtPtr> &stmts) {
  for (const auto& stmt: stmts) {
    Resolve(*stmt);
  }
}

void Resolver::ResolveFunction(const FunctionStmt &stmt, FunctionType type) {
  VariableRestorer restorer(current_function_);
  current_function_ = type;

  BeginScope();
  for (Token param: stmt.params) {
    Declare(param);
    Define(param);
  }

  Resolve(stmt.body);
  EndScope();
}

void Resolver::ResolveLocal(const Expr &expr, Token name) {
  if (scopes_.empty()) return ;
  for (int i = scopes_.size() - 1; i >= 0; --i) {
    if (scopes_[i].contains(name.lexeme())) {
      executor_.Resolve(&expr, scopes_.size() - 1 - i);
      return ;
    }
  }
}

void Resolver::BeginScope() {
  scopes_.emplace_back();
}

void Resolver::EndScope() {
  scopes_.pop_back();
}

void Resolver::Declare(Token name) {
  if (scopes_.empty()) return ;
  auto& scope = scopes_.back();
  if (scope.contains(name.lexeme())) {
    reporter_.set_error(name, "Already a variable with this name in this scope.");
  }
  scope[name.lexeme()] = false;
}

void Resolver::Define(Token name) {
  if (scopes_.empty()) return ;
  auto& scope = scopes_.back();
  scope[name.lexeme()] = true;
}
}