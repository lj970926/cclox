//
// Created by lijin on 23-1-6.
//

#ifndef CCLOX_REPORTER_H
#define CCLOX_REPORTER_H
#include <vector>
#include <string>

namespace cclox {

enum class LoxStatus {
  OK,
  ERROR
};

class ErrorReporter {
 public:
  void set_error(int line, const std::string& message);
  LoxStatus status() const;
  void Print() const;
  void clear();

 private:

  std::vector<std::string> error_msgs_;
  LoxStatus status_ {LoxStatus::OK};
};
}

#endif  // CCLOX_REPORTER_H
