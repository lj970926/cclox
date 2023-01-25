//
// Created by lijin on 23-1-24.
//
#include "resolver.h"

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
  ResolveFunction(stmt);
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
  if (stmt.expr) {
    Resolve(*stmt.expr);
  }
}

void Resolver::VisitWhileStmt(const WhileStmt &stmt) {
  Resolve(*stmt.condition);
  Resolve(*stmt.stmt);
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

void Resolver::ResolveFunction(const FunctionStmt &stmt) {
  BeginScope();
  for (Token param: stmt.params) {
    Declare(param);
    Define(param);
  }

  Resolve(stmt.body);
  EndScope();
}

void Resolver::BeginScope() {
  scopes_.emplace();
}

void Resolver::EndScope() {
  scopes_.pop();
}

void Resolver::Declare(Token name) {
  if (scopes_.empty()) return ;
  auto& scope = scopes_.top();
  scope.emplace(name.lexeme(), false);
}

void Resolver::Define(Token name) {
  if (scopes_.empty()) return ;
  auto& scope = scopes_.top();
  scope.emplace(name.lexeme(), true);
}
}