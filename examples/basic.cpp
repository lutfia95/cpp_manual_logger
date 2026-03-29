#include <exception>
#include <stdexcept>

#include "manuel_logger.hpp"

int main() {
    manuel::ManuelLogger logger("import-job", "debug", "logs/example.log");

    logger.banner("Manual Logger");
    logger.info("Application started");
    logger.success("Connected successfully");
    logger.section("Extract");
    logger.kv("rows_loaded", 128);
    logger.pair("raw.csv", "clean.csv");
    logger.missing("optional field", "middle_name");

    try {
        throw std::runtime_error("sample failure");
    } catch (const std::exception& exception) {
        logger.exception("Processing failed", exception);
    }

    return 0;
}
