#include "grp.h"
#include "keyboard.h"
#include <SDL2/SDL.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <string>
#include <algorithm>

void keyboard_sdl_handle_event(const SDL_Event& ev);

byte dbuffer[64000];
RGBcolor current_palette[256];

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;
static SDL_Texture* screen_tex = nullptr;
static uint32_t pixels[320*200];
static bool sdl_ready = false;

static void die(const char* msg) { std::fprintf(stderr, "%s: %s\n", msg, SDL_GetError()); std::exit(1); }

static std::string clean_name(const char* name) {
    std::string s = name ? name : "";
    while (!s.empty() && (s.back()==' ' || s.back()=='\r' || s.back()=='\n' || s.back()=='\t')) s.pop_back();
    return s;
}
static FILE* open_asset(const char* name) {
    std::string n = clean_name(name);
    FILE* f = std::fopen(n.c_str(), "rb");
    if (!f) f = std::fopen(("assets/" + n).c_str(), "rb");
    if (!f) f = std::fopen(("../assets/" + n).c_str(), "rb");
    return f;
}

static bool read_pcx(FILE* f, byte** out_pixels, RGBcolor** out_pal) {
    PCXheader h{};
    if (std::fread(&h, 1, sizeof(h), f) != sizeof(h)) return false;
    int w = h.Xmax - h.Xmin + 1;
    int ht = h.Ymax - h.Ymin + 1;
    if (w <= 0 || ht <= 0 || h.bit_per_pixel != 8 || h.planes != 1) return false;
    byte* img = (byte*)std::malloc(w * ht);
    if (!img) return false;
    int pos = 0;
    while (pos < w * ht) {
        int c = std::fgetc(f);
        if (c == EOF) { std::free(img); return false; }
        if ((c & 0xC0) == 0xC0) {
            int count = c & 0x3F;
            int val = std::fgetc(f);
            if (val == EOF) { std::free(img); return false; }
            while (count-- && pos < w * ht) img[pos++] = (byte)val;
        } else img[pos++] = (byte)c;
    }
    RGBcolor* pal = nullptr;
    if (out_pal) {
        long cur = std::ftell(f);
        std::fseek(f, -769, SEEK_END);
        int marker = std::fgetc(f);
        if (marker == 12) {
            pal = (RGBcolor*)std::malloc(sizeof(RGBcolor)*256);
            for (int i=0;i<256;i++) {
                pal[i].r=(byte)std::fgetc(f); pal[i].g=(byte)std::fgetc(f); pal[i].b=(byte)std::fgetc(f);
            }
        }
        std::fseek(f, cur, SEEK_SET);
    }
    *out_pixels = img;
    if (out_pal) *out_pal = pal;
    return true;
}

byte* load_pic_from_file(char* filename) {
    FILE* f = open_asset(filename);
    if (!f) { std::fprintf(stderr, "Cannot open image: %s\n", filename); std::exit(2); }
    byte* img = nullptr;
    if (!read_pcx(f, &img, nullptr)) { std::fprintf(stderr, "Bad PCX/PCC: %s\n", filename); std::exit(2); }
    std::fclose(f);
    return img;
}

RGBcolor* load_pal_from_file(char* filename) {
    FILE* f = open_asset(filename);
    if (!f) { std::fprintf(stderr, "Cannot open palette: %s\n", filename); std::exit(2); }
    byte* img = nullptr; RGBcolor* pal = nullptr;
    if (!read_pcx(f, &img, &pal) || !pal) { std::fprintf(stderr, "No palette in: %s\n", filename); std::exit(2); }
    std::free(img); std::fclose(f);
    return pal;
}

void gmode(short mode) {
    if (mode == 0x13) {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) die("SDL_Init failed");
        window = SDL_CreateWindow("Magic Car Pet - 1996 port", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 600, SDL_WINDOW_SHOWN);
        if (!window) die("SDL_CreateWindow failed");
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!renderer) renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
        if (!renderer) die("SDL_CreateRenderer failed");
        screen_tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 320, 200);
        if (!screen_tex) die("SDL_CreateTexture failed");
        sdl_ready = true;
    } else {
        if (screen_tex) SDL_DestroyTexture(screen_tex);
        if (renderer) SDL_DestroyRenderer(renderer);
        if (window) SDL_DestroyWindow(window);
        screen_tex=nullptr; renderer=nullptr; window=nullptr;
        if (sdl_ready) SDL_Quit();
        sdl_ready=false;
    }
}

void setpal(RGBcolor* pal) {
    if (!pal) return;
    for (int i=0;i<256;i++) current_palette[i] = pal[i];
}

void plot(short x, short y, byte color) {
    if (x >= 0 && x < 320 && y >= 0 && y < 200) dbuffer[y*320+x] = color;
}

void update_screen() {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) keyboard_sdl_handle_event(ev);
    for (int i=0;i<320*200;i++) {
        RGBcolor c = current_palette[dbuffer[i]];
        pixels[i] = 0xFF000000u | ((uint32_t)c.r << 16) | ((uint32_t)c.g << 8) | c.b;
    }
    SDL_UpdateTexture(screen_tex, nullptr, pixels, 320 * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, screen_tex, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void line(int x1,int y1,int x2,int y2,char color) {
    int dx = std::abs(x2-x1), sx = x1<x2 ? 1 : -1;
    int dy = -std::abs(y2-y1), sy = y1<y2 ? 1 : -1;
    int err = dx + dy;
    for (;;) {
        plot((short)x1,(short)y1,(byte)color);
        if (x1==x2 && y1==y2) break;
        int e2 = 2*err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
}
