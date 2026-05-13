#include "keyboard.h"
#include <SDL2/SDL.h>

extern "C" volatile char keyboard_map[128] = {0};
extern "C" int keyboard_chain_flag = 0;
extern "C" int keyboard_init(void) { for (int i=0;i<128;i++) keyboard_map[i]=0; return 0; }
extern "C" void keyboard_close(void) {}

static int scan_from_sdl(SDL_Keycode k) {
    switch (k) {
        case SDLK_ESCAPE: return SCAN_ESC;
        case SDLK_UP: return SCAN_UP;
        case SDLK_DOWN: return SCAN_DOWN;
        case SDLK_LEFT: return SCAN_LEFT;
        case SDLK_RIGHT: return SCAN_RIGHT;
        case SDLK_PAGEUP: return SCAN_PGUP;
        case SDLK_PAGEDOWN: return SCAN_PGDN;
        case SDLK_q: return SCAN_Q;
        case SDLK_a: return SCAN_A;
        case SDLK_u: return SCAN_U;
        case SDLK_i: return SCAN_I;
        case SDLK_j: return SCAN_J; // original source checks J, README says I
        default: return -1;
    }
}

void keyboard_sdl_handle_event(const SDL_Event& ev) {
    if (ev.type == SDL_QUIT) keyboard_map[SCAN_ESC] = 1;
    if (ev.type == SDL_KEYDOWN || ev.type == SDL_KEYUP) {
        int sc = scan_from_sdl(ev.key.keysym.sym);
        if (sc >= 0 && sc < 128) keyboard_map[sc] = (ev.type == SDL_KEYDOWN) ? 1 : 0;
    }
}
