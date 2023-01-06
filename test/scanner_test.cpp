//
// Created by lijin on 23-1-6.
//

#include "gtest/gtest.h"

#include <string>
#include <vector>

#include "reporter.h"
#include "scanner.h"
#include "token.h"

namespace cclox {
TEST(ScannerTest, ValidTokens) {
  std::string source
      = "( ) { } , . - + ; / * ! != = == > >= < <= arg1 \"string\" 1 and class "
        "else false fun "
        "for if nil or print return super this true var while";
  ErrorReporter reporter;
  Scanner scanner(source, reporter);
  std::vector<TokenType> expected = {
      TokenType::LEFT_PAREN,
      TokenType::RIGHT_PAREN,
      TokenType::LEFT_BRACE,
      TokenType::RIGHT_BRACE,
      TokenType::COMMA,
      TokenType::DOT,
      TokenType::MINUS,
      TokenType::PLUS,
      TokenType::SEMICOLON,
      TokenType::SLASH,
      TokenType::STAR,
      TokenType::BANG,
      TokenType::BANG_EQUAL,
      TokenType::EQUAL,
      TokenType::EQUAL_EQUAL,
      TokenType::GREATER,
      TokenType::GREATER_EQUAL,
      TokenType::LESS,
      TokenType::LESS_EQUAL,
      TokenType::IDENTIFIER,
      TokenType::STRING,
      TokenType::NUMBER,
      TokenType::AND,
      TokenType::CLASS,
      TokenType::ELSE,
      TokenType::FALSE,
      TokenType::FUN,
      TokenType::FOR,
      TokenType::IF,
      TokenType::NIL,
      TokenType::OR,
      TokenType::PRINT,
      TokenType::RETURN,
      TokenType::SUPER,
      TokenType::THIS,
      TokenType::TRUE,
      TokenType::VAR,
      TokenType::WHILE,
      TokenType::LOX_EOF
  };

  std::list<Token> tokens = scanner.ScanTokens();
  std::vector<Token> tokens_vec {tokens.begin(), tokens.end()};
  ASSERT_EQ(tokens.size(), expected.size());
  for (int i = 0; i < tokens.size(); ++i) {
    EXPECT_EQ(tokens_vec[i].type(), expected[i]);
  }
}

}