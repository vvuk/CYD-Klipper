#ifdef NATIVE_SDL
#include "../screen_driver.h"
#include "../../conf/global_config.h"
#include "lvgl.h"
#include "../lv_setup.h"
#include <SDL2/SDL.h>

static lv_disp_drv_t disp_drv;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[SCREEN_WIDTH * 10];
static lv_indev_drv_t indev_drv;

static void sdl_display_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    SDL_Rect rect;
    rect.x = area->x1;
    rect.y = area->y1;
    rect.w = (area->x2 - area->x1 + 1);
    rect.h = (area->y2 - area->y1 + 1);

    SDL_Surface * screen = SDL_GetWindowSurface((SDL_Window*)disp_drv->user_data);
    
    for(int y = 0; y < rect.h; y++) {
        for(int x = 0; x < rect.w; x++) {
            lv_color_t c = color_p[y * rect.w + x];
            Uint32 pixel = SDL_MapRGB(screen->format, 
                                     LV_COLOR_GET_R(c) << 3,
                                     LV_COLOR_GET_G(c) << 2, 
                                     LV_COLOR_GET_B(c) << 3);
            
            Uint32 * pixels = (Uint32 *)screen->pixels;
            pixels[(rect.y + y) * SCREEN_WIDTH + (rect.x + x)] = pixel;
        }
    }
    
    SDL_UpdateWindowSurfaceRects((SDL_Window*)disp_drv->user_data, &rect, 1);
    lv_disp_flush_ready(disp_drv);
}

static void sdl_mouse_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static bool mouse_pressed = false;
    static lv_coord_t last_x = 0;
    static lv_coord_t last_y = 0;

    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_MOUSEBUTTONDOWN:
                if(event.button.button == SDL_BUTTON_LEFT) {
                    mouse_pressed = true;
                    last_x = event.button.x;
                    last_y = event.button.y;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if(event.button.button == SDL_BUTTON_LEFT) {
                    mouse_pressed = false;
                }
                break;
            case SDL_MOUSEMOTION:
                last_x = event.motion.x;
                last_y = event.motion.y;
                break;
            case SDL_QUIT:
                exit(0);
                break;
        }
    }
    
    data->state = mouse_pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
    data->point.x = last_x;
    data->point.y = last_y;
}

void screen_setBrightness(unsigned char brightness)
{
}

void set_invert_display(){
}

void screen_setup()
{
    lv_init();
   
    SDL_SetHint(SDL_HINT_NO_SIGNAL_HANDLERS, "1");

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return;
    }
    
    SDL_Window * window = SDL_CreateWindow("CYD-Klipper", 
                                          SDL_WINDOWPOS_UNDEFINED, 
                                          SDL_WINDOWPOS_UNDEFINED, 
                                          SCREEN_WIDTH, SCREEN_HEIGHT, 
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Quit();
        return;
    }
    
    SDL_Delay(5);
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, SCREEN_WIDTH * 10);
    
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = SCREEN_WIDTH;
    disp_drv.ver_res = SCREEN_HEIGHT;
    disp_drv.flush_cb = sdl_display_flush;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.user_data = window;
    lv_disp_drv_register(&disp_drv);
    
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = sdl_mouse_read;
    lv_indev_drv_register(&indev_drv);
}

#endif
