#include "../logger.hpp"

int throw_exception() {
  throw 1;
  return 1;
}

int main() {
  char *leak = new char[100];
  throw 1;
}
