#ifdef NATIVE_SDL
#include "../screen_driver.h"
#include "../../conf/global_config.h"
#include "lvgl.h"
#include "../lv_setup.h"

void screen_setBrightness(unsigned char brightness)
{
}

void set_invert_display(){
}

void screen_setup()
{
    lv_init();
}

#endif