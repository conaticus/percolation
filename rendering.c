#include <SDL2/SDL.h>
#include <stdio.h>
#include "rendering.h"

const int WINDOW_SIZE = 600;

// Theme
const SDL_Color COLOR_WHITE = { 255, 255, 255, 255 };
const SDL_Color COLOR_BLACK = { 0, 0, 0, 255 };
const SDL_Color COLOR_BLUE = { 115, 169, 255, 255 };

void sdl_setup_window(SDL_Window** window, SDL_Renderer** renderer) {
    // Todo: Failable
    SDL_Init(SDL_INIT_VIDEO);

    // +1 allows room for the grid border lines
    SDL_CreateWindowAndRenderer(WINDOW_SIZE + 1, WINDOW_SIZE + 1, 0, window, renderer);
    SDL_SetWindowTitle(*window, "Percolation");

    if (window == NULL)
        printf("Could not create window %s\n", SDL_GetError());

    if (renderer == NULL)
        printf("Could not create renderer %s\n", SDL_GetError());
}

void sdl_set_color_white(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, COLOR_WHITE.a);
}

void sdl_set_color_black(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, COLOR_BLACK.r, COLOR_BLACK.g, COLOR_BLACK.b, COLOR_BLACK.a);
}

void sdl_draw_grid_outlines(GridDimensions* grid_dimensions, SDL_Renderer* renderer) {
    sdl_set_color_black(renderer);

    // Vertical
    for (int x = 0; x < 1 + grid_dimensions->width * grid_dimensions->cell_size; x += grid_dimensions->cell_size) {
        SDL_RenderDrawLine(renderer, x, 0, x, WINDOW_SIZE);
    }

    // Horizontal
    for (int y = 0; y < 1 + grid_dimensions->height * grid_dimensions->cell_size; y += grid_dimensions->cell_size) {
        SDL_RenderDrawLine(renderer, 0, y, WINDOW_SIZE, y);
    }
}

int sdl_cleanup(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;
}