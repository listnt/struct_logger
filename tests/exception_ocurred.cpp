#include "../logger.hpp"

int throw_exception() {
  throw 1;
  return 1;
}

int main() {
  zap::logger log;

  try {
    log << "msg" << throw_exception();
    return 0;
  } catch (...) {
    std::cout << "exception ocurred" << std::endl;
    return 0;
  }
}
