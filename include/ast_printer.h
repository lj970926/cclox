//
// Created by lijin on 23-1-10.
//

#ifndef CCLOX_AST_PRINTER_H
#define CCLOX_AST_PRINTER_H
#include "expr.h"

#include <string>
#include <initializer_list>

namespace cclox {
class AstPrinter: public Visitor {
 public:
  const std::string& result() { return result_; }
  const std::string& Print(const Expr& expr) {
    expr.accept(*this);
    return result();
  }
  void VisitBinary(const BinaryExpr& binary) override;
  void VisitGrouping(const GroupingExpr& grouping) override;
  void VisitLiteral(const LiteralExpr& literal) override;
  void VisitUnary(const UnaryExpr& unary) override;

 private:
  std::string result_;
  void Parenthesize(const std::string& name, const std::initializer_list<const Expr*>& exprs);
};

}

#endif  // CCLOX_AST_PRINTER_H