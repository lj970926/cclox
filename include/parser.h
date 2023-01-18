//
// Created by lijin on 23-1-11.
//

#ifndef CCLOX_PARSER_H
#define CCLOX_PARSER_H

#include <vector>
#include <initializer_list>
#include <string>

#include "token.h"
#include "expr.h"
#include "reporter.h"
#include "stmt.h"

namespace cclox {
class ParseError: public std::exception {};

class Parser {
 public:
  Parser(const std::vector<Token>& tokens, ErrorReporter& reporter);
  std::vector<StmtPtr> Parse();
 private:
  std::vector<Token> tokens_;
  size_t current_ = 0;
  ErrorReporter& reporter_;

  ExprPtr Expression();
  ExprPtr Equality();
  ExprPtr Comparison();
  ExprPtr Term();
  ExprPtr Factor();
  ExprPtr Unary();
  ExprPtr Primary();
  ExprPtr Assignment();
  ExprPtr Or();
  ExprPtr And();

  StmtPtr Statement();
  StmtPtr PrintStatement();
  StmtPtr ExpressionStatement();
  StmtPtr Declaration();
  StmtPtr VarDeclaration();
  StmtPtr IfStatement();
  StmtPtr WhileStatement();
  StmtPtr ForStatement();

  std::vector<StmtPtr> Block();

  bool Match(const std::initializer_list<TokenType>& types);
  bool Check(TokenType type) const;
  Token Advance();
  bool End() const;
  Token Peek() const;
  Token Previous() const;

  Token Consume(TokenType type, const std::string& message);
  ParseError Error(Token token, const std::string& message) const;
  void Synchronize();
};
} //namespace cclox

#endif  // CCLOX_PARSER_H
