#ifndef HARDWARESERIAL_H_
#define HARDWARESERIAL_H_

struct SerialType {
    void print(char);
    void print(const char*);
    void println(const char*);
    void printf(const char*, ...);
    int read();
};

extern SerialType Serial;

#endif
