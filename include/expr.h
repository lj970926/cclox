//
// Created by lijin on 23-1-7.
//

#ifndef CCLOX_EXPR_H
#define CCLOX_EXPR_H
#include <memory>
#include <utility>
#include <vector>

#include "common.h"
#include "token.h"

namespace cclox {

struct BinaryExpr;
struct GroupingExpr;
struct LiteralExpr;
struct UnaryExpr;
struct VariableExpr;
struct AssignExpr;
struct LogicExpr;
struct CallExpr;
struct GetExpr;
struct SetExpr;

class ExprVisitor {
 public:
  ExprVisitor() = default;
  virtual ~ExprVisitor() = default;
  virtual void VisitBinary(const BinaryExpr& binary) = 0;
  virtual void VisitGrouping(const GroupingExpr& group) = 0;
  virtual void VisitLiteral(const LiteralExpr& literal) = 0;
  virtual void VisitUnary(const UnaryExpr& unary) = 0;
  virtual void VisitVariable(const VariableExpr& var) = 0;
  virtual void VisitAssign(const AssignExpr& assign) = 0;
  virtual void VisitLogic(const LogicExpr& logic) = 0;
  virtual void VisitCall(const CallExpr& call) = 0;
  virtual void VisitGet(const GetExpr& get) = 0;
  virtual void VisitSet(const SetExpr& set) = 0;
};

struct Expr: NonCopyable {
  virtual void Accept(ExprVisitor& visitor) const = 0;
};

using ExprPtr = std::unique_ptr<Expr>;

struct BinaryExpr: public Expr {
  ExprPtr left;
  Token token;
  ExprPtr right;
  BinaryExpr(ExprPtr l, Token t, ExprPtr r)
      : left(std::move(l)), token(t), right(std::move(r)) {}

  void Accept(ExprVisitor& visitor) const override {
    visitor.VisitBinary(*this);
  }
};

struct GroupingExpr: public Expr {
  ExprPtr expr;
  explicit GroupingExpr(ExprPtr e): expr(std::move(e)) {}

  void Accept(ExprVisitor& visitor) const override {
    visitor.VisitGrouping(*this);
  }
};

struct LiteralExpr: public Expr {
  OptionalLiteral value;
  explicit LiteralExpr(OptionalLiteral v): value(v) {}

  void Accept(ExprVisitor& visitor) const override {
    visitor.VisitLiteral(*this);
  }
};

struct UnaryExpr: public Expr {
  Token token;
  ExprPtr right;
  UnaryExpr(Token t, ExprPtr r): token(t), right(std::move(r)) {}

  void Accept(ExprVisitor& visitor) const override {
    visitor.VisitUnary(*this);
  }
};

struct VariableExpr: public Expr {
  Token name;
  explicit VariableExpr(Token n): name(n) {}

  void Accept(ExprVisitor& visitor) const override {
    visitor.VisitVariable(*this);
  }
};

struct AssignExpr: public Expr {
  Token name;
  ExprPtr expr;
  AssignExpr(Token n, ExprPtr e): name(n), expr(std::move(e)) {}

  void Accept(ExprVisitor& visitor) const override {
    visitor.VisitAssign(*this);
  }
};

struct LogicExpr: public Expr {
  Token op;
  ExprPtr left;
  ExprPtr right;
  LogicExpr(ExprPtr l, Token o, ExprPtr r)
      : left(std::move(l)), op(o), right(std::move(r)) {}

  void Accept(ExprVisitor& visitor) const override {
    visitor.VisitLogic(*this);
  }
};

struct CallExpr: public Expr {
  ExprPtr callee;
  Token paren;
  std::vector<ExprPtr> arguments;

  CallExpr(ExprPtr c, Token p, std::vector<ExprPtr>&& a)
      : callee(std::move(c)), paren(p), arguments(std::move(a)) {}

  void Accept(ExprVisitor& visitor) const override {
    visitor.VisitCall(*this);
  }
};

struct GetExpr: public Expr {
  ExprPtr object;
  Token name;

  GetExpr(ExprPtr o, Token n): object(std::move(o)), name(n) {}

  void Accept(ExprVisitor& visitor) const override {
    visitor.VisitGet(*this);
  }
};

struct SetExpr: public Expr {
  ExprPtr object;
  Token name;
  ExprPtr value;

  SetExpr(ExprPtr o, Token n, ExprPtr v)
      : object(std::move(o)), name(n), value(std::move(v)) {}

  void Accept(ExprVisitor& visitor) const override {
    visitor.VisitSet(*this);
  }
};

} //namespace cclox

#endif  // CCLOX_EXPR_H
