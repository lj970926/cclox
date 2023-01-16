//
// Created by lijin on 23-1-15.
//

#ifndef CCLOX_STMT_H
#define CCLOX_STMT_H
#include <memory>

#include "common.h"
#include "expr.h"

namespace cclox {

struct ExpressionStmt;
struct PrintStmt;
struct VarStmt;

class StmtVisitor {
 public:
  virtual void VisitExpressionStmt(const ExpressionStmt& stmt) = 0;
  virtual void VisitPrintStmt(const PrintStmt& stmt) = 0;
  virtual void VisitVarStmt(const VarStmt& stmt) = 0;
};

struct Stmt: public NonCopyable {
  virtual void Accept(StmtVisitor& visitor) const = 0;
};

using StmtPtr = std::unique_ptr<Stmt>;

struct ExpressionStmt: public Stmt {
  ExprPtr expr;
  explicit ExpressionStmt(ExprPtr e): expr(std::move(e)) {}
  void Accept(StmtVisitor& visitor) const override {
    visitor.VisitExpressionStmt(*this);
  }
};

struct PrintStmt: public Stmt {
  ExprPtr expr;
  explicit PrintStmt(ExprPtr e): expr(std::move(e)) {}
  void Accept(StmtVisitor& visitor) const override {
    visitor.VisitPrintStmt(*this);
  }
};

struct VarStmt: public Stmt {
  Token name;
  ExprPtr init_expr;

  VarStmt(Token n, ExprPtr e): name(n), init_expr(std::move(e)) {}

  void Accept(StmtVisitor& visitor) const override {
    visitor.VisitVarStmt(*this);
  }
};

}

#endif  // CCLOX_STMT_H