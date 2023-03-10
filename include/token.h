//
// Created by lijin on 2023/1/4.
//

#ifndef CCLOX_TOKEN_H
#define CCLOX_TOKEN_H

#include <string>
#include <optional>
#include <variant>
#include <memory>

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

class LoxCallable;
using CallablePtr = std::shared_ptr<LoxCallable>;

class LoxInstance;
using InstancePtr = std::shared_ptr<LoxInstance>;

class LoxClass;
using ClassPtr = std::shared_ptr<LoxClass>;

using Literal = std::variant<std::string, double, CallablePtr, InstancePtr>;
using OptionalLiteral = std::optional<Literal>;

#define DOUBLE_VALUE(literal) (std::get<double>((literal).value()))
#define STRING_VALUE(literal) (std::get<std::string>((literal).value()))
#define CALLABLE_VALUE(literal) (std::get<CallablePtr>((literal).value()))
#define INSTANCE_VALUE(literal) (std::get<InstancePtr>((literal).value()))

#define IS_DOUBLE(literal) ((literal).value().index() == 1)
#define IS_STRING(literal) ((literal).value().index() == 0)
#define IS_CALLABLE(literal) ((literal).value().index() == 2)
#define IS_INSTANCE(literal) ((literal).value().index() == 3)
#define BOOL_STRING(val) ((val) ? "true" : "false")

class Token {
 public:
  Token(TokenType type, const std::string& lexme, const OptionalLiteral& literal, size_t line);
  operator std::string() const;
  TokenType type() const;
  const std::string& lexeme() const;
  const OptionalLiteral& literal() const;
  size_t line() const;
 private:
  TokenType type_;
  size_t line_;
  std::string lexeme_;
  OptionalLiteral literal_;
};

inline std::string GetLiteralStr(const OptionalLiteral& literal) {
  if (!literal.has_value())
    return "";

  if (IS_STRING(literal))
    return std::get<std::string>(literal.value());
  else
    return std::to_string(std::get<double>(literal.value()));
}

} //namespace cclox

#endif  // CCLOX_TOKEN_H
