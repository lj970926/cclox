//
// Created by lijin on 23-1-15.
//

#ifndef CCLOX_STMT_H
#define CCLOX_STMT_H
#include "expr.h"

namespace cclox {

struct ExpressionStmt;
struct PrintStmt;

class StmtVisitor {
 public:
  virtual void VisitExpression(const ExpressionStmt& stmt) = 0;
  virtual void VisitPrint(const PrintStmt& stmt) = 0;
};

struct Stmt {
  virtual void Accept(StmtVisitor& visitor) = 0;
};

struct ExpressionStmt: public Stmt {
  ExprPtr expr;
  void Accept(StmtVisitor& visitor) override {
    visitor.VisitExpression(*this);
  }
};

}

#endif  // CCLOX_STMT_H
