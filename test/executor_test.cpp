//
// Created by lijin on 23-1-14.
//
#include "gtest/gtest.h"

#include <vector>
#include <string>
#include <variant>

#include "token.h"
#include "reporter.h"
#include "scanner.h"
#include "parser.h"
#include "executor.h"
#include "expr.h"

namespace cclox {
TEST(ExecutorTest, ValidExpr) {
  std::vector<std::string> texts {"1 + 1", "2 - 1", "2 * 3", "4 / 2",
                                 "(-3 + 2) * 5", "3 > 2", "1 == 2", "4 >= 2",
                                 "1 != 2", "1 < 2", "1 <= 1", "!nil", "!false",
                                 "!\"fuck\"", "!0", "\"fuck\" + \"you\"", "2 == \"2\""};
  std::vector<OptionalLiteral> expected {{2.0}, {1.0}, {6.0}, {2.0},
                                        {-5.0}, {"true"}, {"false"},
                                        {"true"}, {"true"}, {"true"}, {"true"},
                                        {"true"}, {"true"}, {"false"}, {"false"},
                                        {"fuckyou"}, {"false"}};
  ASSERT_EQ(texts.size(), expected.size());

  for (size_t i = 0; i < texts.size(); ++i) {
    const auto& text =texts[i];
    ErrorReporter reporter;
    Scanner scanner(text, reporter);
    auto tokens = scanner.ScanTokens();
    ASSERT_EQ(reporter.status(), LoxStatus::OK);

    Parser parser(tokens, reporter);
    ExprPtr expr = parser.Parse();
    ASSERT_EQ(reporter.status(), LoxStatus::OK);

    Executor executor(reporter);
    auto res = executor.Execute(expr);
    ASSERT_EQ(reporter.status(), LoxStatus::OK);
    EXPECT_EQ(res, expected[i]) << "i = " << i;
  }
}

TEST(ExecutorTest, InvalidExpr) {
  std::string text = "1 + \"a\"";
  ErrorReporter reporter;
  Scanner scanner(text, reporter);
  auto tokens = scanner.ScanTokens();
  ASSERT_EQ(reporter.status(), LoxStatus::OK);

  Parser parser(tokens, reporter);
  ExprPtr expr = parser.Parse();
  ASSERT_EQ(reporter.status(), LoxStatus::OK);

  Executor executor(reporter);
  auto res = executor.Execute(expr);
  ASSERT_EQ(reporter.status(), LoxStatus::ERROR);
  ASSERT_EQ(res, std::nullopt);

}
} //namespace cclox