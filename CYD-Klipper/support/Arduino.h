#ifndef ARDUINO_H
#define ARDUINO_H

#include "Esp.h"

// Impelemnt Arduino-compatible String class

#include <string>

class String {
public:
    String() : _str("") {}
    String(const char* str) : _str(str) {}
    String(const std::string& s) : _str(s) {}
    String(const String& other) : _str(other._str) {}
    String(String&& other) noexcept : _str(std::move(other._str)) {}
    String(int val) : _str(std::to_string(val)) {}
    String(unsigned int val) : _str(std::to_string(val)) {}

    // Assignment operators
    String& operator=(const String& other) { _str = other._str; return *this; }
    String& operator=(String&& other) noexcept { _str = std::move(other._str); return *this; }
    String& operator=(const char* s) { _str = s; return *this; }
    String& operator=(const std::string& s) { _str = s; return *this; }

    // + operator for String + String
    String operator+(const String& rhs) const { return String(_str + rhs._str); }
    // + operator for String + const char[]
    String operator+(const char* rhs) const { return String(_str + std::string(rhs)); }

    // Allow String + String and String + const char[]
    friend String operator+(const char* lhs, const String& rhs) { return String(std::string(lhs) + rhs._str); }

    // Automatic conversion to const char*
    operator const char*() const { return _str.c_str(); }
    const char* c_str() const { return _str.c_str(); }

    operator const std::string&() const { return _str; }

    // For Arduino compatibility
    size_t length() const { return _str.length(); }
    bool operator==(const String& rhs) const { return _str == rhs._str; }
    bool operator==(const char* rhs) const { return _str == rhs; }
    bool operator!=(const String& rhs) const { return _str != rhs._str; }
    bool operator!=(const char* rhs) const { return _str != rhs; }

private:
    std::string _str;
};

struct StringStream {
    StringStream(const std::string& s) : _s(s) {}
    unsigned int readBytes(char* buffer, unsigned int len) const {
        unsigned int n = std::min(len, (unsigned int)_s.size());
        memcpy(buffer, _s.c_str(), n);
        return n;
    }
    std::string _s;
};
#endif
