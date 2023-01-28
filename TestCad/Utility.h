#pragma once
#include <source_location>
#include <string>

inline std::string GetCurrentLocationInCode(const std::source_location location = std::source_location::current()) {
    std::string message = "file: " +
        static_cast<std::string>(location.file_name())
        + " line: "
        + std::to_string(location.line()-1);
    return message;
}
