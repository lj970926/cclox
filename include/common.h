//
// Created by lijin on 23-1-7.
//

#ifndef CCLOX_COMMON_H
#define CCLOX_COMMON_H

class NonCopyable {
 public:
  NonCopyable() = default;
  virtual ~NonCopyable() = default;
  NonCopyable(const NonCopyable&) = delete;
  NonCopyable& operator=(const NonCopyable&) = delete;
};

#endif  // CCLOX_COMMON_H
