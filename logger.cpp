#include "logger.hpp"

namespace zap {
std::string toLevel(LEVEL lvl)
{
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

logger::logger()
{
    out = &std::cout;
}

logger::logger(std::ostream *out)
{
    out = out;
}

void logger::setLvl(LEVEL lvl)
{
    this->lvl = lvl;
}
void logger::setOstream(std::ostream *out)
{
    this->out = out;
}

logger::~logger()
{
    if (isFlushed && !(*isFlushed)) {
        (*out) << "\b}" << std::endl;
        *isFlushed = true;
    }

    if (arg_count == 1) {
        delete this->isFlushed;
        this->isFlushed = nullptr;
    }
}

} // namespace zap
