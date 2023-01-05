//
// Created by lijin on 23-1-5.
//

#include "scanner.h"

namespace cclox {
namespace {
bool IsDigit(char c) {
  return c >= '0' && c <= '9';
}
}
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

bool Scanner::Match(char expected) {
  if (End())
    return false;
  if (source_[current_] == expected) {
    current_++;
    return true;
  }
  return false;
}

char Scanner::Peak() const {
  if (End())
    return '\0';
  return source_[current_];
}

char Scanner::PeekNext() const {
  if (current_ + 1 >= source_.length()) return '\0';
  return source_[current_ + 1];
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
    case '!':
      AddToken(Match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
      break ;
    case '=':
      AddToken(Match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
      break ;
    case '<':
      AddToken(Match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
      break ;
    case '>':
      AddToken(Match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
      break ;
    case '/':
      if (Match('/')) {
        while (!End() && Peak() != '\n') NextChar();
      } else {
        AddToken(TokenType::SLASH);
      }
      break ;
    case ' ':
    case '\r':
    case '\t':
      break ;
    case '\n':
      ++line_;
      break ;
    case '"':
      AddString();
      break ;
    default:
      if (IsDigit(c)) {
        AddNumber();
      }else {
        //TODO: Report error
      }
  }
}

void Scanner::AddString() {
  while (!End() && Peak() != '"') {
    if (NextChar() == '\n')
      ++line_;
  }

  if (End()) {
    //TODO: Report Error
    return ;
  }

  NextChar();
  OptionalLiteral val = source_.substr(start_ + 1, current_ - start_ - 2);
  AddToken(TokenType::STRING, val);
}

void Scanner::AddNumber() {
  while (IsDigit(Peak())) NextChar();

  if (Peak() == '.' && IsDigit(PeekNext())) {
    NextChar();
    while (IsDigit(Peak())) NextChar();
  }

  OptionalLiteral val = std::stod(source_.substr(start_, current_ - start_));
  AddToken(TokenType::NUMBER, val);
}

} //namespace