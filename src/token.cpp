//
// Created by lijin on 2023/1/4.
//

#include "token.h"
#include <unordered_map>

namespace cclox {
namespace {
  std::unordered_map<TokenType, std::string> token_str {
    {TokenType::LEFT_PAREN, "LEFT_PAREN"},
    {TokenType::RIGHT_PAREN, "RIGHT_PAREN"}
  };

  std::string GetLiteralStr(const OptionalLiteral& literal) {
    int index = literal.value().index();
    if (index == 0)
      return std::get<std::string>(literal.value());
    else
      return std::to_string(std::get<double>(literal.value()));
  }
} //namespace
Token::operator std::string() const {
  const std::string& token_name = token_str.find(type_)->second;
  auto res = token_name + " " + lexeme + " ";
  if (literal_)
    res += GetLiteralStr(literal_);
  return res;
}
}