//
// Created by lijin on 23-1-11.
//

#ifndef CCLOX_PARSER_H
#define CCLOX_PARSER_H

#include <vector>
#include <initializer_list>

#include "token.h"
#include "expr.h"

namespace cclox {
class Parser {
 public:
  Parser(const std::vector<Token>& tokens);
 private:
  std::vector<Token> tokens_;
  size_t current_ = 0;

  ExprPtr Expression();
  ExprPtr Equality();
  ExprPtr Comparison();
  ExprPtr Term();

  bool Match(const std::initializer_list<TokenType>& types);
  bool Check(TokenType type) const;
  Token Advance();
  bool End() const;
  Token Peek() const;
  Token Previous() const;
};
} //namespace cclox

#endif  // CCLOX_PARSER_H
