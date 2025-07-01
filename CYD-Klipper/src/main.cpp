#include "conf/global_config.h"
#include "core/screen_driver.h"
#ifndef NATIVE_SDL
#include "ui/wifi_setup.h"
#include "ui/ip_setup.h"
#include <Esp.h>
#include "ui/ota_setup.h"
#endif
#include "ui/serial/serial_console.h"
#include "lvgl.h"
#include "core/data_setup.h"
#include "ui/main_ui.h"
#include "ui/nav_buttons.h"
#include "core/lv_setup.h"

#ifndef NATIVE_SDL
void setup() {
    Serial.begin(115200);
    serial_console::greet();
    load_global_config();
    screen_setup();
    lv_setup();
    LOG_LN("Screen init done");
    
    wifi_init();
    ota_init();
    ip_init();
    data_setup();

    nav_style_setup();
    main_ui_setup();
}

void loop(){
    wifi_ok();
    data_loop();
    lv_handler();
    serial_console::run();

    if (is_ready_for_ota_update())
    {
        ota_do_update();
    }
}
#else
#define SDL_MAIN_HANDLED 1
#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char **argv) {
    // Native SDL entry point for desktop
    serial_console::greet();
    load_global_config();
    screen_setup();
    lv_setup();
    LOG_LN("Screen init done");
    data_setup();
    nav_style_setup();
    main_ui_setup();

    while (1) {
        lv_handler();
        SDL_Delay(5); // Let the OS breathe
    }
    return 0;
}
#endif