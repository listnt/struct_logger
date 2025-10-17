## Common Module: Logger

Lightweight key-value structured logger. Produces single-line log entries with a timestamp, level and alternating `key=value` pairs.

### Quick start

```cpp
#include "common/logger.hpp"
using namespace zap;

int main() {
    logger log;             // defaults to INFO and std::cout
    log.setLvl(LEVEL::INFO);

    // First argument starts the entry, following args alternate key/value
    log << "event" << "startup" << "pid" << getpid() << "version" << "1.0.0";
    // Example output (millis will vary):
    // {timestamp=1729170000000 lvl=INFO event=startup pid=12345 version=1.0.0}
}
```

### Output format

- Prefix: `{timestamp=<millis_since_epoch> lvl=<DEBUG|INFO|WARN|ERR>`
- Body: alternating arguments are emitted as `key=value` pairs separated by spaces
- The final brace `}` is automatically closed on object destruction

Example:

```text
{timestamp=1729170000000 lvl=INFO event=startup pid=12345 version=1.0.0}
```

### API

- `enum LEVEL { DEBUG, INFO, WARN, ERR }`
- `std::string toLevel(LEVEL lvl)` — converts level to string
- `class logger`
  - `logger()` — logs to `std::cout`
  - `logger(std::ostream* out)` — direct logs to a custom stream
  - `~logger()` — finalizes the log line if not flushed
  - `void setLvl(LEVEL lvl)` — set log severity
  - `void setOstream(std::ostream* out)` — change output stream
  - `operator<<` — stream any type; first arg begins entry, subsequent args alternate key/value

### Usage notes

- Pass arguments in pairs after the first token: `log << "event" << "connected" << "host" << host;`
- You can chain multiple temporary results; the logger ensures a single line is produced.
- Use one `logger` instance per log entry to preserve braces and formatting.

### Files

- `common/logger.hpp` — public API
- `common/logger.cpp` — implementation


