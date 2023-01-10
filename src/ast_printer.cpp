//
// Created by lijin on 23-1-10.
//

#include "ast_printer.h"

#include "token.h"

namespace cclox {
void AstPrinter::VisitBinary(const BinaryExpr &binary) {
  Parenthesize(binary.token.lexeme(), {binary.left.get(), binary.right.get()});
}

void AstPrinter::VisitGrouping(const GroupingExpr &grouping) {
  Parenthesize("group", {grouping.expr.get()});
}

void AstPrinter::VisitLiteral(const LiteralExpr &literal) {
  result_ += GetLiteralStr(literal.value);
}

void AstPrinter::VisitUnary(const UnaryExpr &unary) {
  return Parenthesize(unary.token.lexeme(), {unary.right.get()});
}

void AstPrinter::Parenthesize(const std::string &name, const std::initializer_list<const Expr *> &exprs) {
  result_ = result_ + "(" + name;
  for (const Expr* expr: exprs) {
    result_ += " ";
    expr->accept(*this);
  }
  result_ += ")";
}
}