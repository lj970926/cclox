//
// Created by lijin on 23-1-24.
//

#ifndef CCLOX_RESOLVER_H
#define CCLOX_RESOLVER_H

#include <vector>

#include "expr.h"
#include "stmt.h"

namespace cclox {
class Resolver: public ExprVisitor, public StmtVisitor {
 public:
  void VisitBlockStmt(const BlockStmt& stmt) override;
 private:
  void BeginScope();
  void EndScope();
  void Resolve(const std::vector<StmtPtr>& stmts);
  void Resolve(const Expr& expr);
  void Resolve(const Stmt& stmt);
};
} //namespace cclox

#endif  // CCLOX_RESOLVER_H
