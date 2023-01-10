//
// Created by lijin on 23-1-10.
//

#include "gtest/gtest.h"

#include <memory>
#include <optional>
#include <string>

#include "expr.h"
#include "ast_printer.h"

namespace cclox {
TEST(AstPrinterTest, BasicTest) {
  ExprPtr expr(
      new BinaryExpr(std::make_unique<UnaryExpr>(
          Token(TokenType::MINUS, "-", std::nullopt, 1),
          std::make_unique<LiteralExpr>(123.0)
          ),
	  Token(TokenType::STAR, "*", std::nullopt, 1),
	  std::make_unique<GroupingExpr>(std::make_unique<LiteralExpr>(45.67))));
  std::string expected = "(* (- 123.000000) (group 45.670000))";
  AstPrinter printer;
  EXPECT_EQ(printer.Print(*expr.get()), expected);
}
}
