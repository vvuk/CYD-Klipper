#ifndef ESP_H
#define ESP_H

#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
#include <limits>
#include <algorithm>

struct ESPType {
    void restart() {}
};

extern ESPType ESP;

extern "C" {
#endif

// implement millis and delay
unsigned long millis();
void delay(unsigned long ms);
int esp_get_free_heap_size();

#ifdef __cplusplus
}
#endif

#endif
