#include <SDL2/SDL.h>
#include <stdio.h>
#include "rendering.h"

const int WINDOW_SIZE = 600;
const int BORDER_SIZE = 1;

// Theme
const SDL_Color COLOR_WHITE = { 255, 255, 255, 255 };
const SDL_Color COLOR_BLACK = { 0, 0, 0, 255 };

void sdl_setup_window(SDL_Window** window, SDL_Renderer** renderer) {
    // Todo: Failable
    SDL_Init(SDL_INIT_VIDEO);

    SDL_CreateWindowAndRenderer(WINDOW_SIZE + BORDER_SIZE, WINDOW_SIZE + BORDER_SIZE, 0, window, renderer);
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

void sdl_poke_hole(SDL_Renderer* renderer, SDL_Rect* rect) {
    sdl_set_color_white(renderer);
    SDL_RenderFillRect(renderer, rect);
}

void sdl_messagebox_warning(SDL_Window* window, char* message) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Percolation", message, window);
}

int sdl_cleanup(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;
}