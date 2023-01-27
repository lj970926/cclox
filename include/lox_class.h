//
// Created by lijin on 23-1-27.
//

#ifndef CCLOX_LOX_CLASS_H
#define CCLOX_LOX_CLASS_H

#include <string>

namespace cclox {
class LoxClass {
 public:
  LoxClass(const std::string& name): name_(name) {}
  std::string name() const {return name_;}
 private:
  std::string name_;
};
}

#endif  // CCLOX_LOX_CLASS_H
