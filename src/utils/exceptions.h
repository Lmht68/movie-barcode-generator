#ifndef UTILS_EXCEPTIONS_H_
#define UTILS_EXCEPTIONS_H_

#include <stdexcept>
#include <string>

namespace app_exception {
    class LoggerInitException : public std::runtime_error {
    public:
        explicit LoggerInitException(const std::string& message) : std::runtime_error(message) {}
    };

    class StyleInitException : public std::runtime_error {
    public:
        explicit StyleInitException(const std::string& message) : std::runtime_error(message) {}
    };
}  // namespace app_exception

#endif