//
// Created by lijin on 23-1-13.
//

#ifndef CCLOX_EXECUTOR_H
#define CCLOX_EXECUTOR_H
#include <initializer_list>
#include <vector>

#include "expr.h"
#include "token.h"
#include "reporter.h"
#include "stmt.h"

namespace cclox {
class Executor: public ExprVisitor, public StmtVisitor{
 public:
  Executor(ErrorReporter& reporter);
  void VisitBinary(const BinaryExpr& expr) override;
  void VisitGrouping(const GroupingExpr& expr) override;
  void VisitLiteral(const LiteralExpr& expr) override;
  void VisitUnary(const UnaryExpr& expr) override;

  void VisitExpressionStmt(const ExpressionStmt& stmt) override;
  void VisitPrintStmt(const PrintStmt& stmt) override;

  void Execute(const std::vector<StmtPtr>& stmts);

 private:
  OptionalLiteral value_;
  ErrorReporter& reporter_;

  OptionalLiteral EvaluateExpr(const Expr& expr);
  void EvaluateStmt(const Stmt& stmt);
  bool IsTruthy(OptionalLiteral value) const;
  bool IsEqual(OptionalLiteral lhs, OptionalLiteral rhs) const;
  void CheckNumberOperand(Token op, const std::initializer_list<OptionalLiteral>& values) const;
};
} //namespace cclox

#endif  // CCLOX_EXECUTOR_H
