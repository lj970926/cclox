#include <iostream>
#include <cstdlib>

#include "interpreter.h"

int main(int argc, char* argv[]) {
  cclox::Interpreter interpreter;
  if (argc > 2) {
    std::cout << "Usage: " << argv[0] << " [script]\n";
    std::exit(64);
  } else if (argc == 2) {
    interpreter.RunFile(argv[1]);
  } else {
    interpreter.RunPrompt();
  }
  return 0;
}
