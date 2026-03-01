#ifndef UTILS_EXCEPTIONS_H_
#define UTILS_EXCEPTIONS_H_

#include <stdexcept>
#include <string>

namespace app_exception {
    class AppException : public std::runtime_error {
    public:
        explicit AppException(const std::string& message) : std::runtime_error(message) {}
    };

    class LoggerInitException : public AppException {
    public:
        using AppException::AppException;
    };

    class StyleInitException : public AppException {
    public:
        using AppException::AppException;
    };
}  // namespace app_exception

#endif