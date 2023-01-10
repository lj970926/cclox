//
// Created by lijin on 23-1-10.
//

#include "ast_printer.h"

namespace cclox {
void AstPrinter::VisitBinary(const BinaryExpr &binary) {
  return parenthesize(binary.token.lexeme(), {*binary.left.get(), *binary.right.get()});
}

void AstPrinter::parenthesize(const std::string &name, const std::initializer_list<const Expr&> exprs) {

}
}