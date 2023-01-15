//
// Created by lijin on 23-1-11.
//

#include "parser.h"

#include <memory>
#include <utility>

namespace cclox {
Parser::Parser(const std::vector<Token> &tokens, ErrorReporter& reporter)
    : tokens_(tokens), reporter_(reporter) {}

std::vector<StmtPtr> Parser::Parse() {
  std::vector<StmtPtr> stmts;
  try {
    while (!End()) {
      stmts.emplace_back(Statement());
    }
  } catch (const ParseError& error) {
    return {};
  }
  return stmts;
}

StmtPtr Parser::Statement() {
  if (Match({TokenType::PRINT})) {
    return PrintStatement();
  }

  return ExpressionStatement();
}

StmtPtr Parser::PrintStatement() {
  ExprPtr expr = Expression();
  Consume(TokenType::SEMICOLON, "Expect ';' after value.");
  return std::make_unique<PrintStmt>(std::move(expr));
}

StmtPtr Parser::ExpressionStatement() {
  ExprPtr expr = Expression();
  Consume(TokenType::SEMICOLON, "Expect ';' after value.");
  return std::make_unique<ExpressionStmt>(std::move(expr));
}
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

ExprPtr Parser::Term() {
  ExprPtr expr = Factor();

  while (Match({TokenType::MINUS, TokenType::PLUS})) {
    Token op = Previous();
    ExprPtr right = Factor();
    expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
  }

  return expr;
}

ExprPtr Parser::Factor() {
  ExprPtr expr = Unary();

  while (Match({TokenType::STAR, TokenType::SLASH})) {
    Token op = Previous();
    ExprPtr right = Unary();
    expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
  }
  return expr;
}

ExprPtr Parser::Unary() {
  if (Match({TokenType::MINUS, TokenType::BANG})) {
    Token op = Previous();
    ExprPtr right = Unary();
    return std::make_unique<UnaryExpr>(op, std::move(right));
  }

  return Primary();
}

ExprPtr Parser::Primary() {
  if (Match({TokenType::FALSE})) return std::make_unique<LiteralExpr>("false");
  if (Match({TokenType::TRUE})) return std::make_unique<LiteralExpr>("true");
  if (Match({TokenType::NIL})) return std::make_unique<LiteralExpr>("nil");

  if (Match({TokenType::NUMBER, TokenType::STRING})) {
    return std::make_unique<LiteralExpr>(Previous().literal());
  }

  if (Match({TokenType::LEFT_PAREN})) {
    ExprPtr expr = Expression();
    Consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
    return std::make_unique<GroupingExpr>(std::move(expr));
  }
  throw Error(Peek(), "Expect expression.");
}

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

Token Parser::Consume(TokenType type, const std::string &message) {
  if (Check(type)) return Advance();
  throw Error(Peek(), message);
}

ParseError Parser::Error(cclox::Token token, const std::string &message) const {
  std::string error_msg;
  if (token.type() == TokenType::LOX_EOF) {
    error_msg = "at end, " + message;
  } else {
    error_msg = "at '" + token.lexeme() + "', " + message;
  }

  reporter_.set_error(token.line(), error_msg);
  return {};
}
} //namespace cclox