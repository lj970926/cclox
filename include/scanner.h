//
// Created by lijin on 23-1-5.
//

#ifndef CCLOX_SCANNER_H
#define CCLOX_SCANNER_H
#include <string>
#include <list>

#include "token.h"

namespace cclox {
class Scanner {
 public:
  Scanner(const std::string source="");
  std::list<Token> ScanTokens();
 private:
  std::string source_;
};
}

#endif  // CCLOX_SCANNER_H
