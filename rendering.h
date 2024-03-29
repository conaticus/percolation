#ifndef RENDERING_H
#define RENDERING_H

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct {
    int virtual_size;
    int cell_size;

    bool is_valid;
} GridDimensions;

typedef enum {
    Blue,
    White,
} Color;

const int WINDOW_SIZE;

void sdl_setup_window(SDL_Window** window, SDL_Renderer** renderer);

void sdl_messagebox_warning(SDL_Window* window, char* message);
void sdl_poke_hole(SDL_Renderer* renderer, SDL_Rect* rect, Color color);

int sdl_cleanup(SDL_Window* window, SDL_Renderer* renderer);

#endif // RENDERING_H