//
// Created by lijin on 23-1-13.
//

#ifndef CCLOX_EXECUTOR_H
#define CCLOX_EXECUTOR_H
#include <initializer_list>
#include <vector>
#include <memory>

#include "expr.h"
#include "token.h"
#include "reporter.h"
#include "stmt.h"
#include "environment.h"

namespace cclox {
class Executor: public ExprVisitor, public StmtVisitor{
 public:
  Executor(ErrorReporter& reporter);
  void VisitBinary(const BinaryExpr& expr) override;
  void VisitGrouping(const GroupingExpr& expr) override;
  void VisitLiteral(const LiteralExpr& expr) override;
  void VisitUnary(const UnaryExpr& expr) override;
  void VisitVariable(const VariableExpr& expr) override;
  void VisitAssign(const AssignExpr& expr) override;
  void VisitLogic(const LogicExpr& expr) override;

  void VisitExpressionStmt(const ExpressionStmt& stmt) override;
  void VisitPrintStmt(const PrintStmt& stmt) override;
  void VisitVarStmt(const VarStmt& stmt) override;
  void VisitBlockStmt(const BlockStmt& stmt) override;
  void VisitIfStmt(const IfStmt& stmt) override;
  void VisitWhileStmt(const WhileStmt& stmt) override;

  void Execute(const std::vector<StmtPtr>& stmts);
  OptionalLiteral Execute(const ExprPtr& expr);
  void ExecuteBlock(const std::vector<StmtPtr>& stmts, std::shared_ptr<Environment> environment);
 private:
  OptionalLiteral value_;
  ErrorReporter& reporter_;
  std::shared_ptr<Environment> environment_;

  OptionalLiteral EvaluateExpr(const Expr& expr);
  void EvaluateStmt(const Stmt& stmt);
  bool IsTruthy(OptionalLiteral value) const;
  bool IsEqual(OptionalLiteral lhs, OptionalLiteral rhs) const;
  void CheckNumberOperand(Token op, const std::initializer_list<OptionalLiteral>& values) const;
};
} //namespace cclox

#endif  // CCLOX_EXECUTOR_H
