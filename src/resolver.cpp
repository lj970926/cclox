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

void Resolver::VisitVariable(const VariableExpr &expr) {
  if (InInitializer(expr.name.lexeme())) {
    reporter_.set_error(expr.name, "Can't read local variable in its own initializer.");
  }

  ResolveLocal(expr, expr.name);
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