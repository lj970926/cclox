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

class StmtVisitor {
 public:
  virtual void VisitExpressionStmt(const ExpressionStmt& stmt) = 0;
  virtual void VisitPrintStmt(const PrintStmt& stmt) = 0;
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

}

#endif  // CCLOX_STMT_H
