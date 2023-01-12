//
// Created by lijin on 23-1-12.
//

#include "gtest/gtest.h"

#include <string>
#include <optional>

#include "expr.h"
#include "token.h"
#include "scanner.h"
#include "reporter.h"
#include "parser.h"
#include "ast_printer.h"

namespace cclox {
TEST(ParserTest, BasicTest) {
  std::string code = "1 + 1\n";

  ExprPtr expected = std::make_unique<BinaryExpr>(std::make_unique<LiteralExpr>(1.0),
      Token(TokenType::PLUS, "+", std::nullopt, 1), std::make_unique<LiteralExpr>(1.0));

  ErrorReporter reporter;
  Scanner scanner(code, reporter);
  auto tokens = scanner.ScanTokens();
  ASSERT_EQ(tokens.size(), 4);

  Parser parser(tokens, reporter);
  ExprPtr expr = parser.Parse();

  AstPrinter printer;
  ASSERT_EQ(printer.Print(*expr.get()), printer.Print(*expected.get()));
}
} //namespace cclox