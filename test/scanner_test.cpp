//
// Created by lijin on 23-1-6.
//

#include "gtest/gtest.h"

#include <string>

namespace cclox {
TEST(ScannerTest, ValidTokens) {
  std::string source
      = "( ) { } , . - + ; / * ! != = == > >= < <= arg1 \"string\" 1 and class "
        "else false fun "
        "for if nil or print return super this true var while";
}

}