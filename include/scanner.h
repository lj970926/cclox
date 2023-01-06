//
// Created by lijin on 23-1-5.
//

#ifndef CCLOX_SCANNER_H
#define CCLOX_SCANNER_H
#include <string>
#include <list>

#include "token.h"
#include "reporter.h"

namespace cclox {
class Scanner {
 public:
  Scanner(const std::string& source, ErrorReporter& reporter);
  std::list<Token> ScanTokens();
 private:
  std::string source_;
  size_t start_ = 0;
  size_t current_ = 0;
  size_t line_ = 1;
  std::list<Token> tokens_;
  ErrorReporter& reporter_;

  bool End() const;
  char NextChar();
  void ScanToken();
  void AddToken(TokenType type);
  void AddToken(TokenType type, const OptionalLiteral &literal);
  bool Match(char expected);
  char Peak() const;
  char PeekNext() const;
  void AddString();
  void AddNumber();
  void AddIdentifier();
};
}

#endif  // CCLOX_SCANNER_H
