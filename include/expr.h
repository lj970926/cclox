//
// Created by lijin on 23-1-7.
//

#ifndef CCLOX_EXPR_H
#define CCLOX_EXPR_H
#include <memory>
#include <utility>

#include "common.h"
#include "token.h"

namespace cclox {

struct BinaryExpr;
struct GroupingExpr;
struct LiteralExpr;
struct UnaryExpr;

class Visitor {
 public:
  Visitor() = default;
  virtual ~Visitor() = default;
  virtual void VisitBinary(const BinaryExpr& binary) = 0;
  virtual void VisitGrouping(const GroupingExpr& group) = 0;
  virtual void VisitLiteral(const LiteralExpr& literal) = 0;
  virtual void VisitUnary(const UnaryExpr& unary) = 0;
};

struct Expr: NonCopyable {
  virtual void accept(Visitor&& visitor) const = 0;
};

using ExprPtr = std::unique_ptr<Expr>;

struct BinaryExpr: public Expr {
  ExprPtr left;
  Token token;
  ExprPtr right;
  BinaryExpr(ExprPtr l, Token t, ExprPtr r)
      : left(std::move(l)), token(t), right(std::move(r)) {}

  void accept(Visitor&& visitor) const override {
    visitor.VisitBinary(*this);
  }
};

class GroupingExpr: public Expr {
  ExprPtr expr;
  explicit GroupingExpr(ExprPtr e): expr(std::move(e)) {}

  void accept(Visitor&& visitor) const override {
    visitor.VisitGrouping(*this);
  }
};

class LiteralExpr: public Expr {
  OptionalLiteral value;
  explicit LiteralExpr(OptionalLiteral v): value(v) {}

  void accept(Visitor&& visitor) const override {
    visitor.VisitLiteral(*this);
  }
};

class UnaryExpr: public Expr {
  Token token;
  ExprPtr right;
  UnaryExpr(Token t, ExprPtr r): token(t), right(std::move(r)) {}

  void accept(Visitor&& visitor) const override {
    visitor.VisitUnary(*this);
  }
};

} //namespace cclox

#endif  // CCLOX_EXPR_H
