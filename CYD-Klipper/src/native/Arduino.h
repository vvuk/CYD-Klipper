#ifndef ARDUINO_H
#define ARDUINO_H

#include "Esp.h"

// Impelemnt Arduino-compatible String class

#ifdef __cplusplus
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

    String& operator+=(const String& rhs) { _str += rhs._str; return *this; }
    String& operator+=(const char* rhs) { _str += std::string(rhs); return *this; }
    String& operator+=(char rhs) { _str += rhs; return *this; }

    // Automatic conversion to const char*
    operator const char*() const { return _str.c_str(); }
    const char* c_str() const { return _str.c_str(); }

    operator const std::string&() const { return _str; }
    operator std::string&() { return _str; }

    // For Arduino compatibility
    size_t length() const { return _str.length(); }
    bool operator==(const String& rhs) const { return _str == rhs._str; }
    bool operator==(const char* rhs) const { return _str == rhs; }
    bool operator!=(const String& rhs) const { return _str != rhs._str; }
    bool operator!=(const char* rhs) const { return _str != rhs; }

    int toInt() const { return std::stoi(_str); }
    float toFloat() const { return std::stof(_str); }

private:
    std::string _str;
};

struct Print {
    void save(const unsigned char*p) {}
};

struct StringStream : public Print {
    StringStream(const std::string& s) : _off(0), _s(s) {}

    unsigned int readBytes(unsigned char* buffer, unsigned int len) {
        unsigned int n = std::min(len, (unsigned int)_s.size() - _off);
        memcpy(buffer, _s.c_str() + _off, n);
        _off += n;
        return n;
    }
    unsigned int readBytes(char* buffer, unsigned int len) {
        return readBytes((unsigned char*) buffer, len);
    }
    int read() {
        if (_off == _s.size())
            return -1;
        return _s[_off++];
    }
    char* s() { return (char*) _s.c_str(); }
    operator const std::string&() const { return _s; }
    operator std::string() const { return _s; }
    operator String() const { return String(_s); }
    operator const char*() const { return (char*) _s.c_str(); }
    operator const unsigned char*() const { return (unsigned char*) _s.c_str(); }

    unsigned int _off;
    std::string _s;
};

struct Printable {
    void printTo(Print& p) const  {}
    bool overflowed() const { return false; }
};
#endif

#endif
