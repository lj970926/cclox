//
// Created by lijin on 23-1-28.
//

#ifndef CCLOX_LOX_INSTANCE_H
#define CCLOX_LOX_INSTANCE_H
#include <string>
#include <unordered_map>

#include "token.h"

namespace cclox {

class LoxInstance {
 public:
  LoxInstance(ClassPtr lox_class): lox_class_(lox_class) {}
  operator std::string () const { return lox_class_->name() + " instance"; }
  OptionalLiteral Get(Token name) const;
 private:
   ClassPtr lox_class_;
   std::unordered_map<std::string, OptionalLiteral> fields_;
};
} //namespace cclox

#endif  // CCLOX_LOX_INSTANCE_H
