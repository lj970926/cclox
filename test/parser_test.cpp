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
#include "stmt.h"


namespace cclox {
TEST(ParserTest, BasicTest) {
  std::string code = "1 + 1;\n";

  ExprPtr expected = std::make_unique<BinaryExpr>(std::make_unique<LiteralExpr>(1.0),
      Token(TokenType::PLUS, "+", std::nullopt, 1), std::make_unique<LiteralExpr>(1.0));

  ErrorReporter reporter;
  Scanner scanner(code, reporter);
  auto tokens = scanner.ScanTokens();
  ASSERT_EQ(tokens.size(), 5);

  Parser parser(tokens, reporter);
  auto stmts = parser.Parse();
  ASSERT_EQ(reporter.status(), LoxStatus::OK);
  ASSERT_EQ(stmts.size(), 1);
  AstPrinter printer;

  auto stmt = dynamic_cast<ExpressionStmt*>(stmts[0].get());
  ASSERT_EQ(printer.Print(*stmt->expr), printer.Print(*expected.get()));
}

TEST(ParserTest, MultipleToken) {
  std::string code = "(-3 - 2) * 8;\n";
  ExprPtr expected = std::make_unique<BinaryExpr>(
      std::make_unique<GroupingExpr>(
          std::make_unique<BinaryExpr>(
              std::make_unique<UnaryExpr>(
                  Token(TokenType::MINUS, "-", std::nullopt, 1),
                  std::make_unique<LiteralExpr>(3.0)
                  ),
              Token(TokenType::MINUS, "-", std::nullopt, 1),
              std::make_unique<LiteralExpr>(2.0)
              )
          ),
      Token(TokenType::STAR, "*", std::nullopt, 1),
      std::make_unique<LiteralExpr>(8.0)
      );

  ErrorReporter reporter;
  Scanner scanner(code, reporter);
  auto tokens = scanner.ScanTokens();
  ASSERT_EQ(tokens.size(), 10);

  Parser parser(tokens, reporter);
  auto stmts = parser.Parse();
  ASSERT_EQ(reporter.status(), LoxStatus::OK);
  ASSERT_EQ(stmts.size(), 1);

  AstPrinter printer;
  auto stmt = dynamic_cast<ExpressionStmt*>(stmts[0].get());
  ASSERT_EQ(printer.Print(*stmt->expr), printer.Print(*expected.get()));
}

TEST(ParserTest, EqualCompAndLiteral) {
  std::string code = "(\"a\" > 3) == true;\n";
  ExprPtr expected = std::make_unique<BinaryExpr>(
      std::make_unique<GroupingExpr>(
          std::make_unique<BinaryExpr>(
              std::make_unique<LiteralExpr>("a"),
              Token(TokenType::GREATER, ">", std::nullopt, 1),
              std::make_unique<LiteralExpr>(3.0)
              )
          ),
      Token(TokenType::EQUAL_EQUAL, "==", std::nullopt, 1),
      std::make_unique<LiteralExpr>("true")
      );

  ErrorReporter reporter;
  Scanner scanner(code, reporter);
  auto tokens = scanner.ScanTokens();
  ASSERT_EQ(tokens.size(), 9);

  Parser parser(tokens, reporter);
  auto stmts = parser.Parse();
  ASSERT_EQ(reporter.status(), LoxStatus::OK);
  ASSERT_EQ(stmts.size(), 1);

  AstPrinter printer;
  auto stmt = dynamic_cast<ExpressionStmt*>(stmts[0].get());
  ASSERT_EQ(printer.Print(*stmt->expr), printer.Print(*expected.get()));
}

TEST(ParserTest, BasicError) {
  std::string code = "(3;\n";
  ErrorReporter reporter;
  Scanner scanner(code, reporter);
  auto tokens = scanner.ScanTokens();
  ASSERT_EQ(tokens.size(), 4);
  Parser parser(tokens, reporter);
  auto stmts = parser.Parse();
  ASSERT_EQ(reporter.status(), LoxStatus::ERROR);
  ASSERT_EQ(stmts.size(), 0);
}
} //namespace cclox