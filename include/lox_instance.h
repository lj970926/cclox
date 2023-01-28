//
// Created by lijin on 23-1-28.
//

#ifndef CCLOX_LOX_INSTANCE_H
#define CCLOX_LOX_INSTANCE_H

#include "token.h"

namespace cclox {

class LoxInstance {
 public:
  LoxInstance(ClassPtr lox_class): lox_class_(lox_class) {}
 private:
   ClassPtr lox_class_;
};
} //namespace cclox

#endif  // CCLOX_LOX_INSTANCE_H
