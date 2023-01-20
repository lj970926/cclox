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
  while (!End()) {
    stmts.emplace_back(Declaration());
  }

  return stmts;
}

StmtPtr Parser::Declaration() {
  try {
    if (Match({TokenType::VAR})) return VarDeclaration();
    if (Match({TokenType::FUN})) return Function("function");
    return Statement();
  } catch (const ParseError& error) {
    Synchronize();
    return nullptr;
  }
}

StmtPtr Parser::VarDeclaration() {
  Token name = Consume(TokenType::IDENTIFIER, "Expect variable name.");
  ExprPtr init_expr = nullptr;
  if (Match({TokenType::EQUAL})) {
    init_expr = Expression();
  }
  Consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
  return std::make_unique<VarStmt>(name, std::move(init_expr));
}

StmtPtr Parser::Function(const std::string &kind){
  Token name = Consume(TokenType::IDENTIFIER, "Expect " + kind + " name.");
  Consume(TokenType::LEFT_PAREN, "Expect '(' after " + kind + " name.");
  std::vector<Token> params;

  if (!Check(TokenType::RIGHT_PAREN)) {
    do {
      if (params.size() >= 255)
        Error(Peek(), "Can not have more than 255 parameters.");
      Token param = Consume(TokenType::IDENTIFIER, "Expect parameter name.");
      params.push_back(param);
    } while (Match({TokenType::COMMA}));
  }

  Consume(TokenType::RIGHT_PAREN, "Expect ')' after parameters.");
  Consume(TokenType::LEFT_BRACE, "Expect '{' before " + kind + " body.");
  auto body = Block();
  return std::make_unique<FunctionStmt>(name, params, std::move(body));
}

StmtPtr Parser::Statement() {
  if (Match({TokenType::IF}))
    return IfStatement();

  if (Match({TokenType::PRINT}))
    return PrintStatement();

  if (Match({TokenType::WHILE}))
    return WhileStatement();

  if (Match({TokenType::FOR}))
    return ForStatement();

  if (Match({TokenType::RETURN}))
    return ReturnStatement();

  if (Match({TokenType::LEFT_BRACE}))
    return std::make_unique<BlockStmt>(Block());

  return ExpressionStatement();
}

StmtPtr Parser::ForStatement() {
  Consume(TokenType::LEFT_PAREN, "Expect '(' after for.");
  StmtPtr init;

  if (Match({TokenType::SEMICOLON})) {
    init = nullptr;
  } else if (Match({TokenType::VAR})) {
    init = VarDeclaration();
  } else {
    init = ExpressionStatement();
  }

  ExprPtr cond = nullptr;
  if (!Check(TokenType::SEMICOLON)) {
    cond = Expression();
  }
  Consume(TokenType::SEMICOLON, "Expect ';' after loop condition.");

  ExprPtr increment = nullptr;
  if (!Check(TokenType::RIGHT_PAREN)) {
    increment = Expression();
  }
  Consume(TokenType::RIGHT_PAREN, "Expect ')' after for clauses.");

  StmtPtr body = Statement();
  if (increment) {
    std::vector<StmtPtr> vec;
    vec.push_back(std::move(body));
    vec.push_back(std::make_unique<ExpressionStmt>(std::move(increment)));
    body = std::make_unique<BlockStmt>(std::move(vec));
  }

  if (!cond) cond = std::make_unique<LiteralExpr>("true");
  body = std::make_unique<WhileStmt>(std::move(cond), std::move(body));

  if (init) {
    std::vector<StmtPtr> vec;
    vec.push_back(std::move(init));
    vec.push_back(std::move(body));
    body = std::make_unique<BlockStmt>(std::move(vec));
  }

  return body;
}

StmtPtr Parser::IfStatement() {
  Consume(TokenType::LEFT_PAREN, "Expect '(' after if.");
  ExprPtr condition = Expression();
  Consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition.");
  StmtPtr then_branch = Statement();
  StmtPtr else_branch = nullptr;
  if (Match({TokenType::ELSE})) {
    else_branch = Statement();
  }

  return std::make_unique<IfStmt>(std::move(condition), std::move(then_branch), std::move(else_branch));
}

StmtPtr Parser::WhileStatement() {
  Consume(TokenType::LEFT_PAREN, "Expect '(' after while.");
  ExprPtr condition = Expression();
  Consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");
  StmtPtr stmt = Statement();

  return std::make_unique<WhileStmt>(std::move(condition), std::move(stmt));
}

StmtPtr Parser::PrintStatement() {
  ExprPtr expr = Expression();
  Consume(TokenType::SEMICOLON, "Expect ';' after value.");
  return std::make_unique<PrintStmt>(std::move(expr));
}

StmtPtr Parser::ReturnStatement() {
  Token keyword = Previous();
  ExprPtr expr = nullptr;
  if (!Check(TokenType::SEMICOLON)) {
    expr = Expression();
  }

  Consume(TokenType::SEMICOLON, "Expect ';' after return value.");
  return std::make_unique<ReturnStmt>(keyword, std::move(expr));
}

StmtPtr Parser::ExpressionStatement() {
  ExprPtr expr = Expression();
  Consume(TokenType::SEMICOLON, "Expect ';' after value.");
  return std::make_unique<ExpressionStmt>(std::move(expr));
}

std::vector<StmtPtr> Parser::Block() {
  std::vector<StmtPtr> stmts;
  while (!End() && !Check(TokenType::RIGHT_BRACE)) {
    stmts.emplace_back(Declaration());
  }

  Consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");
  return stmts;
}

ExprPtr Parser::Expression() {
  return Assignment();
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

  return Call();
}

ExprPtr Parser::Call() {
  ExprPtr expr = Primary();

  while (true) {
    if (Match({TokenType::LEFT_PAREN})) {
      expr = FinishCall(std::move(expr));
    } else {
      break ;
    }
  }
  return expr;
}

ExprPtr Parser::Primary() {
  if (Match({TokenType::FALSE})) return std::make_unique<LiteralExpr>("false");
  if (Match({TokenType::TRUE})) return std::make_unique<LiteralExpr>("true");
  if (Match({TokenType::NIL})) return std::make_unique<LiteralExpr>("nil");

  if (Match({TokenType::NUMBER, TokenType::STRING})) {
    return std::make_unique<LiteralExpr>(Previous().literal());
  }

  if (Match({TokenType::IDENTIFIER}))
    return std::make_unique<VariableExpr>(Previous());

  if (Match({TokenType::LEFT_PAREN})) {
    ExprPtr expr = Expression();
    Consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
    return std::make_unique<GroupingExpr>(std::move(expr));
  }
  throw Error(Peek(), "Expect expression.");
}

ExprPtr Parser::Assignment() {
  ExprPtr expr = Or();

  if (Match({TokenType::EQUAL})) {
    Token equal = Previous();
    ExprPtr value = Assignment();

    auto var = dynamic_cast<VariableExpr*>(expr.get());
    if (var) {
      return std::make_unique<AssignExpr>(var->name, std::move(value));
    }

    Error(equal, "Invalid assignment target.");
  }

  return expr;
}

ExprPtr Parser::Or() {
  ExprPtr expr = And();

  while (Match({TokenType::OR})) {
    Token op = Previous();
    ExprPtr right = And();
    expr = std::make_unique<LogicExpr>(std::move(expr), op, std::move(right));
  }

  return expr;
}

ExprPtr Parser::And() {
  ExprPtr expr = Equality();

  while (Match({TokenType::AND})) {
    Token op = Previous();
    ExprPtr right = Equality();
    expr = std::make_unique<LogicExpr>(std::move(expr), op, std::move(right));
  }

  return expr;
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

void Parser::Synchronize() {
  Advance();
  while (!End()) {
    if (Previous().type() == TokenType::SEMICOLON) return ;

    switch (Peek().type()) {
      case TokenType::CLASS:
      case TokenType::FUN:
      case TokenType::VAR:
      case TokenType::FOR:
      case TokenType::IF:
      case TokenType::WHILE:
      case TokenType::PRINT:
      case TokenType::RETURN:
        return ;
      default:
        break ;
    }

    Advance();
  }
}

ExprPtr Parser::FinishCall(ExprPtr callee) {
  std::vector<ExprPtr> arguments;
  if (!Check(TokenType::RIGHT_PAREN)) {
    do {
      if (arguments.size() >= 255)
        Error(Peek(), "Can't have more than 255 arguments.");

      arguments.push_back(Expression());
    } while (Match({TokenType::COMMA}));
  }

  Token paren = Consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");
  return std::make_unique<CallExpr>(std::move(callee), paren, std::move(arguments));
}

} //namespace cclox