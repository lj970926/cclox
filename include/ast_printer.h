//
// Created by lijin on 23-1-10.
//

#ifndef CCLOX_AST_PRINTER_H
#define CCLOX_AST_PRINTER_H
#include "expr.h"

#include <string>

namespace cclox {
class AstPrinter: public Visitor {
 public:
  const std::string& result() { return result_; }

 private:
  std::string result_;
};
}

#endif  // CCLOX_AST_PRINTER_H
