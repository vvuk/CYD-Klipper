#include <stdio.h>
#include <stdarg.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

#include <SDL2/SDL.h>
#include "Esp.h"
#include "HardwareSerial.h"

ESPType ESP;
SerialType Serial;

SerialType::SerialType() {
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    
    tty.c_lflag &= ~(ICANON | ECHO);
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 0;
    
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
    
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}

void SerialType::print(char c) {
    write(STDOUT_FILENO, &c, 1);
}

void SerialType::print(const char* str) {
    write(STDOUT_FILENO, str, strlen(str));
}
void SerialType::println(const char* str) {
    write(STDOUT_FILENO, str, strlen(str));
    write(STDOUT_FILENO, "\n", 1);
}

void SerialType::printf(const char* fmt, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    if (len > 0) {
        write(STDOUT_FILENO, buffer, len);
    }
}

int SerialType::read() {
    char c;
    ssize_t result = ::read(STDIN_FILENO, &c, 1);
    if (result == 1) {
        return (unsigned char)c;
    }
    return -1;
}

unsigned long millis() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (unsigned long)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void delay(unsigned long ms) {
    usleep(ms * 1000);
}

int esp_get_free_heap_size() {
    return 1<<20;
}

