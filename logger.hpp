#ifndef LOGGER_H
#define LOGGER_H

#include <chrono>
#include <iostream>
#include <mutex>
#include <unordered_map>

namespace zap {
enum LEVEL {
  DEBUG,
  INFO,
  WARN,
  ERR,
};

std::string toLevel(LEVEL lvl) {
  switch (lvl) {
  case LEVEL::DEBUG:
    return "DEBUG";
  case LEVEL::INFO:
    return "INFO";
  case LEVEL::WARN:
    return "WARN";
  case LEVEL::ERR:
    return "ERR";
  default:
    return "";
  }
}

class OStreamLockTable {
private:
  std::unordered_map<std::ostream *, std::shared_ptr<std::mutex>> locks_;
  std::mutex table_mutex_;

public:
  std::shared_ptr<std::mutex> get_lock(std::ostream *os) {
    std::lock_guard<std::mutex> guard(table_mutex_);
    auto it = locks_.find(os);
    if (it == locks_.end()) {
      auto new_lock = std::make_shared<std::mutex>();
      locks_[os] = new_lock;
      return new_lock;
    }
    return it->second;
  }

  static OStreamLockTable &instance() {
    static OStreamLockTable instance_;
    return instance_;
  }
};

class logger {
private:
  LEVEL lvl = INFO;
  bool *isFlushed = nullptr;
  long arg_count = 0;
  std::ostream *out;
  std::ostringstream *tmp; // need for thread safety
public:
  logger() : out(&std::cout){};
  logger(std::ostream *out) : out(out){};
  ~logger() {
    if (isFlushed && !(*isFlushed)) {
      auto lock = OStreamLockTable::instance().get_lock(out);
      std::lock_guard<std::mutex> guard(*lock);

      (*this->tmp) << "\b}";
      (*out) << (*this->tmp).str() << std::endl;
      *isFlushed = true;
    }

    if (arg_count == 1) {
      delete this->isFlushed;
      delete this->tmp;
    }
  };

  void setLvl(LEVEL lvl) { this->lvl = lvl; };
  void setOstream(std::ostream *out) { this->out = out; };

  template <typename T> logger operator<<(T arg) {
    auto res = *this;
    res.arg_count++;
    res.isRoot = false;

    if (res.arg_count == 1) {
      res.isFlushed = new bool(false);
      res.tmp = new std::ostringstream();

      auto now = std::chrono::system_clock::now();

      // Convert to millis since epoch
      auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(
                        now.time_since_epoch())
                        .count();
      std::string prefix = "{timestamp=";
      (*res.tmp) << prefix << millis << " ";

      (*res.tmp) << "lvl=" << toLevel(this->lvl) << " ";
    }

    const char postfix = arg_count % 2 ? ' ' : '=';

    if (postfix == ' ' && (std::is_same<std::decay_t<T>, char *>::value ||
                           std::is_same<std::decay_t<T>, const char *>::value ||
                           std::is_same<std::decay_t<T>, std::string>::value)) {
      (*res.tmp) << "\"" << arg << "\"" << postfix;
    } else {
      (*res.tmp) << arg << postfix;
    }

    return res;
  }
};

} // namespace zap

#endif // LOGGER_H
