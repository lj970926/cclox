//
// Created by lijin on 23-1-27.
//

#ifndef CCLOX_LOX_CLASS_H
#define CCLOX_LOX_CLASS_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include "callable.h"
#include "token.h"

namespace cclox {
class LoxClass: public LoxCallable{
 public:
  LoxClass(const std::string& name, std::shared_ptr<LoxClass> superclass,
           const std::unordered_map<std::string, CallablePtr>& methods)
      : name_(name), superclass_(superclass), methods_(methods) {}

  std::string name() const {return name_;}

  OptionalLiteral Call(Executor& executor, const std::vector<OptionalLiteral>& arguments) override;
  size_t Arity() override;

  CallablePtr FindMethod(const std::string& name) const;

  operator std::string () { return name_; }
 private:
  std::string name_;
  std::shared_ptr<LoxClass> superclass_;
  std::unordered_map<std::string, CallablePtr> methods_;
};
}

#endif  // CCLOX_LOX_CLASS_H
