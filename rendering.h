#ifndef RENDERING_H
#define RENDERING_H

#include <SDL2/SDl.h>
#include <stdbool.h>

// The width and height are virtual, and not actual pixel values.
typedef struct {
    int width;
    int height;
    int cell_size;

    bool is_valid;
} GridDimensions;

const int WINDOW_SIZE;

void sdl_setup_window(SDL_Window** window, SDL_Renderer** renderer);

void sdl_messagebox_warning(SDL_Window* window, char* message);
void sdl_poke_hole(SDL_Renderer* renderer, SDL_Rect* rect);
void sdl_draw_grid_outlines(GridDimensions* grid_dimensions, SDL_Renderer* renderer);

int sdl_cleanup(SDL_Window* window, SDL_Renderer* renderer);

#endif // RENDERING_H