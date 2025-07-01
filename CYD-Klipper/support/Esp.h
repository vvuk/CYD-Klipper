#ifndef ESP_H
#define ESP_H

#include <limits>
#include <algorithm>
#include <stdlib.h>
#include <string.h>

struct ESPType {
    void restart() {}
};

extern ESPType ESP;

// implement millis and delay
unsigned long millis() {
    return 0;
}
void delay(unsigned long ms) {
}

int esp_get_free_heap_size() {
    return 1<<20;
}

#endif
