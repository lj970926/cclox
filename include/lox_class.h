//
// Created by lijin on 23-1-27.
//

#ifndef CCLOX_LOX_CLASS_H
#define CCLOX_LOX_CLASS_H

#include <string>
#include <vector>
#include <memory>

#include "callable.h"
#include "token.h"

namespace cclox {
class LoxClass: public LoxCallable, public std::enable_shared_from_this<LoxClass>{
 public:
  LoxClass(const std::string& name): name_(name) {}
  std::string name() const {return name_;}

  OptionalLiteral Call(Executor& executor, const std::vector<OptionalLiteral>& arguments) override;
  size_t Arity() override;
 private:
  std::string name_;
};
}

#endif  // CCLOX_LOX_CLASS_H
