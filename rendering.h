#ifndef RENDERING_H
#define RENDERING_H

#include <SDL2/SDl.h>
#include <stdbool.h>

typedef struct {
    int width;
    int height;
    int cell_size;

    bool is_valid;
} GridDimensions;

const int WINDOW_SIZE;

void sdl_setup_window(SDL_Window** window, SDL_Renderer** renderer);

void sdl_draw_grid_outlines(GridDimensions* grid_dimensions, SDL_Renderer* renderer);

int sdl_cleanup(SDL_Window* window, SDL_Renderer* renderer);

#endif // RENDERING_H