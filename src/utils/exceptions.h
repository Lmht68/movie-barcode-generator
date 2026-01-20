#include <stdexcept>
#include <string>

namespace AppException {
    class LoggerInitException : public std::runtime_error {
    public:
        explicit LoggerInitException(const std::string& message) : std::runtime_error(message) {}
    };

    class StyleInitException : public std::runtime_error {
    public:
        explicit StyleInitException(const std::string& message) : std::runtime_error(message) {}
    };
}  // namespace AppException
