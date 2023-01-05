//
// Created by lijin on 2023/1/4.
//

#ifndef CCLOX_TOKEN_H
#define CCLOX_TOKEN_H

#include <string>
#include <optional>
#include <variant>

namespace cclox {
enum class TokenType {
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  COMMA,
  DOT,
  MINUS,
  PLUS,
  SEMICOLON,
  SLASH,
  STAR,
  BANG,
  BANG_EQUAL,
  EQUAL,
  EQUAL_EQUAL,
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,
  IDENTIFIER,
  STRING,
  NUMBER,
  AND,
  CLASS,
  ELSE,
  FALSE,
  FUN,
  FOR,
  IF,
  NIL,
  OR,
  PRINT,
  RETURN,
  SUPER,
  THIS,
  TRUE,
  VAR,
  WHILE,
  LOX_EOF
};

using Literal = std::variant<std::string, double>;
using OptionalLiteral = std::optional<Literal>;

class Token {
 public:
  Token(TokenType type, const std::string& lexme, const OptionalLiteral& literal, size_t line);
  operator std::string() const;
 private:
  TokenType type_;
  size_t line_;
  std::string lexeme_;
  OptionalLiteral literal_;
};

} //namespace cclox

#endif  // CCLOX_TOKEN_H
