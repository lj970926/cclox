//
// Created by lijin on 2023/1/4.
//

#include "token.h"
#include <unordered_map>

namespace cclox {
namespace {
  std::unordered_map<TokenType, std::string> token_str {
    {TokenType::LEFT_PAREN, "LEFT_PAREN"},
    {TokenType::RIGHT_PAREN, "RIGHT_PAREN"},
    {TokenType::LEFT_BRACE, "LEFT_BRACE"},
    {TokenType::RIGHT_BRACE, "RIGHT_BRACE"},
    {TokenType::COMMA, "COMMA"},
    {TokenType::DOT, "DOT"},
    {TokenType::MINUS, "MINUS"},
    {TokenType::PLUS, "PLUS"},
    {TokenType::SEMICOLON, "SEMICOLON"},
    {TokenType::SLASH, "SLASH"},
    {TokenType::STAR, "STAR"},
    {TokenType::BANG, "BANG"},
    {TokenType::BANG_EQUAL, "BANG_EQUAL"},
    {TokenType::EQUAL, "EQUAL"},
    {TokenType::EQUAL_EQUAL, "EQUAL_EQUAL"},
    {TokenType::GREATER, "GREATER"},
    {TokenType::GREATER_EQUAL, "GREATER_EQUAL"},
    {TokenType::LESS, "LESS"},
    {TokenType::LESS_EQUAL, "LESS_EQUAL"},
    {TokenType::IDENTIFIER, "IDENTIFIER"},
    {TokenType::STRING, "STRING"},
    {TokenType::NUMBER, "NUMBER"},
    {TokenType::AND, "AND"},
    {TokenType::CLASS, "CLASS"},
    {TokenType::ELSE, "ELSE"},
    {TokenType::FALSE, "FALSE"},
    {TokenType::FUN, "FUN"},
    {TokenType::FOR, "FOR"},
    {TokenType::IF, "IF"},
    {TokenType::NIL, "NIL"},
    {TokenType::OR, "OR"},
    {TokenType::PRINT, "PRINT"},
    {TokenType::RETURN, "RETURN"},
    {TokenType::SUPER, "SUPER"},
    {TokenType::THIS, "THIS"},
    {TokenType::TRUE, "TRUE"},
    {TokenType::VAR, "VAR"},
    {TokenType::WHILE, "WHILE"},
    {TokenType::LOX_EOF, "EOF"}
  };

  std::string GetLiteralStr(const OptionalLiteral& literal) {
    int index = literal.value().index();
    if (index == 0)
      return std::get<std::string>(literal.value());
    else
      return std::to_string(std::get<double>(literal.value()));
  }
} //namespace
Token::operator std::string() const {
  const std::string& token_name = token_str.find(type_)->second;
  auto res = token_name + " " + lexeme_ + " ";
  if (literal_)
    res += GetLiteralStr(literal_);
  return res;
}

Token::Token(TokenType type, const std::string& lexme, const OptionalLiteral& literal, size_t line)
: type_(type), lexeme_(lexme), literal_(literal), line_(line){}

TokenType Token::type() const {
  return type_;
}

const std::string& Token::lexeme() const {
  return lexeme_;
}

} //namespace cclox