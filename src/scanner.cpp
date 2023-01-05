//
// Created by lijin on 23-1-5.
//

#include "scanner.h"

namespace cclox {
Scanner::Scanner(const std::string source): source_(source) {}
std::list<Token> Scanner::ScanTokens() {
  while (!End()) {
    start_ = current_;
    ScanToken();
  }

  tokens_.emplace_back(TokenType::LOX_EOF, "", std::nullopt, line_);
  return tokens_;
}

bool Scanner::End() const {
  return current_ >= source_.length();
}

char Scanner::NextChar() {
  return source_[current_++];
}

void Scanner::AddToken(TokenType type) {
  AddToken(type, std::nullopt);
}

void Scanner::AddToken(TokenType type, const OptionalLiteral &literal) {
  std::string text = source_.substr(start_, current_ - start_);
  tokens_.emplace_back(type, text, literal, line_);
}

void Scanner::ScanToken() {
  char c = NextChar();
  switch (c) {
    case '(': 
      AddToken(TokenType::LEFT_PAREN); 
      break;
    case ')': 
      AddToken(TokenType::RIGHT_PAREN);
      break;
    case '{':
      AddToken(TokenType::LEFT_BRACE);
      break ;
    case '}':
      AddToken(TokenType::RIGHT_BRACE);
      break ;
    case ',':
      AddToken(TokenType::COMMA);
      break ;
    case '.':
      AddToken(TokenType::DOT);
      break ;
    case '-':
      AddToken(TokenType::MINUS);
      break ;
    case '+':
      AddToken(TokenType::PLUS);
      break ;
    case ';':
      AddToken(TokenType::SEMICOLON);
      break ;
    case '*':
      AddToken(TokenType::STAR);
      break ;
  }
}
} //namespace