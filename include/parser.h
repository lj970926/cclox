//
// Created by lijin on 23-1-11.
//

#ifndef CCLOX_PARSER_H
#define CCLOX_PARSER_H

#include <list>

#include "token.h"

namespace cclox {
class Parser {
 public:
  Parser(const std::list<Token>& tokens);
 private:
  std::list<Token> tokens_;
};
}

#endif  // CCLOX_PARSER_H
