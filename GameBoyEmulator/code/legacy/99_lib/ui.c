/**
 ******************************************************************************
 * @file    ui.c
 * @author  Bellino Francesco
 * @brief   User Interface source code.
 *
 ******************************************************************************
 */

#include "../98_include/ui.h"

#include "../05_sdl/sdl2/include/SDL2/SDL.h"
#include "../05_sdl/sdl2_ttf/include/SDL2/SDL_ttf.h"

SDL_Window *sdlWindow;
SDL_Renderer *sdlRenderer;
SDL_Texture *sdlTexture;
SDL_Surface *screen;

void ui_init() {
    
    SDL_Init(SDL_INIT_VIDEO);
    printf("SDL INIT\n");
    TTF_Init();
    printf("TTF INIT\n");

    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &sdlWindow, &sdlRenderer);
}

void delay(u32 ms) {
    SDL_Delay(ms);
}

void ui_handle_events() {
    SDL_Event e;
    while (SDL_PollEvent(&e) > 0)
    {
        //TO DO SDL_UpdateWindowSurface(sdlWindow);
        //TO DO SDL_UpdateWindowSurface(sdlTraceWindow);
        //TO DO SDL_UpdateWindowSurface(sdlDebugWindow);

        if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE) {
            emu_get_context()->die = true;
        }
    }
}
