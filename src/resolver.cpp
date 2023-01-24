//
// Created by lijin on 23-1-24.
//
#include "resolver.h"

namespace cclox {
void Resolver::VisitBlockStmt(const BlockStmt &stmt) {
  BeginScope();
  for (const auto& sub_stmt: stmt.stmts) {
    Resolve(*sub_stmt);
  }
  EndScope();
}

void Resolver::Resolve(const Stmt &stmt) {
  stmt.Accept(*this);
}

void Resolver::Resolve(const Expr &expr) {
  expr.Accept(*this);
}
}