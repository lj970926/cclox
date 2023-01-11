//
// Created by lijin on 23-1-11.
//

#include "parser.h"

#include <memory>
#include <utility>

namespace cclox {
Parser::Parser(const std::vector<Token> &tokens): tokens_(tokens) {}

ExprPtr Parser::Expression() {
  return Equality();
}

ExprPtr Parser::Equality() {
  ExprPtr expr = Comparison();

  while (Match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
    Token op = Previous();
    ExprPtr right = Comparison();
    expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
  }

  return expr;
}

ExprPtr Parser::Comparison() {
  ExprPtr expr = Term();

  while (Match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL})) {
    Token op = Previous();
    ExprPtr right = Term();
    expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
  }

  return expr;
}

ExprPtr Parser::Term() {}

bool Parser::Match(const std::initializer_list<TokenType> &types) {
  for (auto type: types) {
    if (Check(type)) {
      Advance();
      return true;
    }
  }

  return false;
}

bool Parser::Check(TokenType type) const {
  return Peek().type() == type;
}

Token Parser::Advance() {
  if (!End()) ++current_;
  return Previous();
}

Token Parser::Previous() const {
  return tokens_[current_ - 1];
}

Token Parser::Peek() const {
  return tokens_[current_];
}

bool Parser::End() const {
  return Peek().type() == TokenType::LOX_EOF;
}
} //namespace cclox