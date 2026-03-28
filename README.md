# ManuelLogger for C++

Small header-only C++ logger with a Python-like API, backed by `spdlog`.

It mirrors the ergonomics of `py_manual_logger`:

- readable terminal output
- timestamps
- colored levels
- app name on every line
- optional file logging
- convenience helpers like `banner()`, `section()`, `kv()`, `pair()`, and `missing()`

## Features

- one header: [`include/manuel_logger.hpp`](/Users/ahmadlutfi/Downloads/loggers/cpp_manual_logger/include/manuel_logger.hpp)
- header-only CMake target: `manuel_logger::manuel_logger`
- uses installed `spdlog` or fetches it automatically during CMake configure
- log levels: `debug`, `info`, `success`, `warning`, `error`, `critical`
- environment override with `MANUEL_LOG_LEVEL`

## Quick Start

```cpp
#include "manuel_logger.hpp"

int main() {
    manuel::ManuelLogger logger("my-app", "info");

    logger.info("Application started");
    logger.success("Connected successfully");
    logger.warning("Retrying request");
    logger.error("Something failed");
}
```

Write logs to a file too:

```cpp
manuel::ManuelLogger logger("my-app", "info", "logs/app.log");
logger.success("This goes to stderr and logs/app.log");
```

Control the level from your shell:

```bash
export MANUEL_LOG_LEVEL=debug
```

Then in C++:

```cpp
manuel::ManuelLogger logger("worker");
logger.debug("Debug logging is enabled");
```

## API

```cpp
manuel::ManuelLogger logger(
    "app-name",     // logger name
    "info",         // level
    "logs/app.log", // optional file path
    true,           // use_color when auto_detect_color is false
    true,           // auto_detect_color
    true,           // show_time
    true            // show_level
);
```

Available methods:

- `debug(message)`
- `info(message)`
- `success(message)`
- `warning(message)`
- `error(message)`
- `critical(message)`
- `banner(title)`
- `section(title)`
- `kv(key, value, level)`
- `pair(left, right, arrow, level)`
- `missing(what, value)`
- `exception(message, exception)`
- `set_level(level)`

## CMake Usage

### Option 1: Add this repo as a subdirectory

```cmake
add_subdirectory(cpp_manual_logger)

target_link_libraries(your_app PRIVATE manuel_logger::manuel_logger)
```

### Option 2: Install and use `find_package`

Build and install:

```bash
cmake -S cpp_manual_logger -B build
cmake --build build
cmake --install build --prefix /usr/local
```

Then in your project:

```cmake
find_package(manuel_logger REQUIRED)
target_link_libraries(your_app PRIVATE manuel_logger::manuel_logger)
```

### `spdlog` resolution

The CMake build tries this order:

1. use an installed `spdlog`
2. fetch `spdlog` automatically with `FetchContent`

If you want to forbid downloading dependencies:

```bash
cmake -S cpp_manual_logger -B build -DMANUEL_LOGGER_FETCH_SPDLOG=OFF
```

## Example

A small example app is included at [`examples/basic.cpp`](/Users/ahmadlutfi/Downloads/loggers/cpp_manual_logger/examples/basic.cpp).

Build it:

```bash
cmake -S cpp_manual_logger -B build
cmake --build build --target manuel_logger_example
./build/manuel_logger_example
```

## Helper Examples

```cpp
logger.section("Extract");
logger.info("Reading source file");

logger.section("Transform");
logger.pair("raw.csv", "clean.csv");

logger.kv("rows_processed", 128);
logger.kv("environment", "production");

logger.missing("optional field", "middle_name");
```

Exception logging:

```cpp
try {
    throw std::runtime_error("database unavailable");
} catch (const std::exception& exception) {
    logger.exception("Import failed", exception);
}
```

## Notes

- the logger writes to `stderr` for console output, similar to the Python version
- file output is plain text without ANSI colors
- `success` is treated as a level between `info` and `warning`
