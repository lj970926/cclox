//
// Created by lijin on 23-1-7.
//

#ifndef CCLOX_COMMON_H
#define CCLOX_COMMON_H

namespace cclox {
class NonCopyable {
 public:
  NonCopyable() = default;
  virtual ~NonCopyable() = default;
  NonCopyable(const NonCopyable&) = delete;
  NonCopyable& operator=(const NonCopyable&) = delete;
};

template<typename T>
class VariableRestorer {
 public:
  explicit VariableRestorer(T& ref): ref_(ref), save_(ref) {}
  ~VariableRestorer() { ref_ = save_; }

 private:
  T& ref_;
  T save_;
};
}

#endif  // CCLOX_COMMON_H
