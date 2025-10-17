#ifndef LOGGER_H
#define LOGGER_H

#include <QWidget>
#include <iostream>

namespace zap {
enum LEVEL {
    DEBUG,
    INFO,
    WARN,
    ERR,
};

std::string toLevel(LEVEL lvl);

class logger
{
private:
    LEVEL lvl = INFO;
    bool isRoot = true;
    bool *isFlushed = nullptr;
    long arg_count = 0;
    std::ostream *out;

public:
    logger();
    logger(std::ostream *out);
    ~logger();

    void setLvl(LEVEL lvl);
    void setOstream(std::ostream *out);

    template<typename T>
    logger operator<<(T arg){
        if (this->isRoot) {
            auto now = std::chrono::system_clock::now();

            // Convert to seconds since epoch
            auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch())
                              .count();
            std::string prefix = "{timestamp=";
            (*out) << prefix << millis << " ";

            (*out)<<"lvl="<<toLevel(this->lvl)<<" ";
        }

        if (arg_count==1){
            this->isFlushed = new bool(false);
        }

        auto postfix = arg_count % 2 ? ' ' : '=';
        (*out) << arg << postfix;

        bool tmp = this->isRoot;
        this->isRoot = false;

        auto res = *this;
        res.arg_count++;
        this->isRoot = tmp;

        return res;
    }
};
} // namespace zap

#endif // LOGGER_H
