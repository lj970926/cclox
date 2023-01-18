//
// Created by lijin on 23-1-15.
//

#ifndef CCLOX_STMT_H
#define CCLOX_STMT_H
#include <memory>
#include <vector>

#include "common.h"
#include "expr.h"

namespace cclox {

struct ExpressionStmt;
struct PrintStmt;
struct VarStmt;
struct BlockStmt;
struct IfStmt;
struct WhileStmt;

class StmtVisitor {
 public:
  virtual void VisitExpressionStmt(const ExpressionStmt& stmt) = 0;
  virtual void VisitPrintStmt(const PrintStmt& stmt) = 0;
  virtual void VisitVarStmt(const VarStmt& stmt) = 0;
  virtual void VisitBlockStmt(const BlockStmt& stmt) = 0;
  virtual void VisitIfStmt(const IfStmt& stmt) = 0;
  virtual void VisitWhileStmt(const WhileStmt& stmt) = 0;
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

struct BlockStmt: public Stmt {
  std::vector<StmtPtr> stmts;

  BlockStmt(std::vector<StmtPtr>&& sts): stmts(std::move(sts)) {}

  void Accept(StmtVisitor& visitor) const override {
    visitor.VisitBlockStmt(*this);
  }
};

struct IfStmt: public Stmt {
  ExprPtr condition;
  StmtPtr then_branch;
  StmtPtr else_branch;

  IfStmt(ExprPtr c, StmtPtr t, StmtPtr e): condition(std::move(c)), then_branch(std::move(t)), else_branch(std::move(e)) {}

  void Accept(StmtVisitor& visitor) const override {
    visitor.VisitIfStmt(*this);
  }
};

struct WhileStmt: public Stmt {
  ExprPtr condition;
  StmtPtr stmt;

  WhileStmt(ExprPtr c, StmtPtr st): condition(std::move(c)), stmt(std::move(st)) {}

  void Accept(StmtVisitor& visitor) const override {
    visitor.VisitWhileStmt(*this);
  }
};

}

#endif  // CCLOX_STMT_H
