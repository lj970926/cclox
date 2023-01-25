//
// Created by lijin on 23-1-24.
//

#ifndef CCLOX_RESOLVER_H
#define CCLOX_RESOLVER_H

#include <vector>
#include <stack>
#include <unordered_map>
#include <string>

#include "token.h"
#include "expr.h"
#include "stmt.h"
#include "executor.h"
#include "reporter.h"

namespace cclox {
class Resolver: public ExprVisitor, public StmtVisitor {
 public:

  Resolver(Executor& executor, ErrorReporter& reporter)
      : executor_(executor), reporter_(reporter) {};

  void VisitBlockStmt(const BlockStmt& stmt) override;
  void VisitVarStmt(const VarStmt& stmt) override;
  void VisitFunctionStmt(const FunctionStmt& stmt) override;
  void VisitExpressionStmt(const ExpressionStmt& stmt) override;
  void VisitIfStmt(const IfStmt& stmt) override;
  void VisitPrintStmt(const PrintStmt& stmt) override;
  void VisitReturnStmt(const ReturnStmt& stmt) override;
  void VisitWhileStmt(const WhileStmt& stmt) override;

  void VisitVariable(const VariableExpr& expr) override;
  void VisitBinary(const BinaryExpr& expr) override;
  void VisitCall(const CallExpr& expr) override;
  void VisitGrouping(const GroupingExpr& expr) override;
  void VisitLiteral(const LiteralExpr& expr) override;
  void VisitLogic(const LogicExpr& expr) override;
  void VisitUnary(const UnaryExpr& expr) override;

 private:
  void BeginScope();
  void EndScope();
  void Resolve(const std::vector<StmtPtr>& stmts);
  void Resolve(const Expr& expr);
  void Resolve(const Stmt& stmt);
  void ResolveFunction(const FunctionStmt& stmt);
  void ResolveLocal(const Expr& expr, Token name);

  void Declare(Token name);
  void Define(Token name);

  bool InInitializer(const std::string& name) const {
    if (scopes_.empty())
      return false;
    auto& scope = scopes_.top();
    return scope.contains(name) && !scope.at(name);
  }

  std::stack<std::unordered_map<std::string, bool>> scopes_;
  Executor& executor_;
  ErrorReporter& reporter_;
};
} //namespace cclox

#endif  // CCLOX_RESOLVER_H
