#pragma once
#include <stdexcept>

class InvalidRowSizeException : public std::runtime_error {
public:
    // Constructor que toma un mensaje de error como argumento
    explicit InvalidRowSizeException(const std::string& message) : std::runtime_error(message) {}
};
