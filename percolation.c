#include <SDL2/SDL.h>
#include <stdbool.h>
#include "percolation.h"
#include "rendering.h"

void run_simulation(GridDimensions* grid_dimensions, SDL_Renderer* renderer) {
    // Place the grid cursor in the middle of the grid.
    SDL_Rect grid_cursor = {
        .x = (grid_dimensions->width - 1) / 2 * grid_dimensions->cell_size,
        .y = (grid_dimensions->height - 1) / 2 * grid_dimensions->cell_size,
        .w = grid_dimensions->cell_size,
        .h = grid_dimensions->cell_size,
    };

    // SDL_Rect* highlighted_squares = malloc(grid_dimensions->width * grid_dimensions->height * sizeof(SDL_Rect));
    // int highlighted_square_count = 0;

    bool quit = false;
    bool mouse_clicked = false;

    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_MOUSEMOTION:
                    grid_cursor.x = (event.motion.x / grid_dimensions->cell_size) * grid_dimensions->cell_size;
                    grid_cursor.y = (event.motion.y / grid_dimensions->cell_size) * grid_dimensions->cell_size;

                    break;
                case SDL_MOUSEBUTTONDOWN:
                    mouse_clicked = true;
                    break;
                case SDL_QUIT:
                    quit = true;
                    break;
            }
        }


        // if (mouse_clicked)
        //     highlighted_squares[highlighted_square_count++] = grid_cursor;

        // for (int i = 0; i < highlighted_square_count; i++) {
        //     // Todo: Check if rect is already in the array (should probably initialise a static array where they are either on or off)

        //     SDL_SetRenderDrawColor(renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, COLOR_WHITE.a);
        //     SDL_RenderFillRect(renderer, &highlighted_squares[i]);
        // }

        // Todo: Optimise this so that it doesn't draw the grid every loop (just shrink the size of the grid_cursor by a pixel and render once) - is that even possible?

        sdl_draw_grid_outlines(grid_dimensions, renderer);

        SDL_RenderPresent(renderer);
        mouse_clicked = false;
    }

    // free(highlighted_squares);
}