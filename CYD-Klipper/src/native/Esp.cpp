#include <stdio.h>
#include <stdarg.h>

#include "Esp.h"
#include "HardwareSerial.h"

ESPType ESP;
SerialType Serial;

void SerialType::print(char c) {
    printf("%c\n", c);
}

void SerialType::print(const char* str) {
    printf("%s\n", str);
}
void SerialType::println(const char* str) {
    printf("%s\n", str);
}

void SerialType::printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

int SerialType::read() {
    return getc(stdin);
}

unsigned long millis() {
    return 0;
}
void delay(unsigned long ms) {
}

int esp_get_free_heap_size() {
    return 1<<20;
}

