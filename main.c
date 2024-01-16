#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "rendering.h"
#include "percolation.h"

// Todo: Test if it works with uneven dimensions. If it doesn't, error if the dimensions are not equal.
// Todo: Add a "mode" CLI option that allows for random holes to be poked (maybe time the interval based on the size of the grid)
// Todo: Figure out the O notation lol
// Todo: Fix rendering bug with black bar on right of screen
// Todo: Remove quit variable, exit instead

void get_grid_dimensions(int argc, char* argv[], GridDimensions* grid_dimensions) {
    if (argc - 1 < 2) {
        printf("Please provide a width and height for the percolation graph.\n");
        return;
    }

    grid_dimensions->width = strtol(argv[1], NULL, 10);
    grid_dimensions->height = strtol(argv[2], NULL, 10);

    if (grid_dimensions->width > 200 || grid_dimensions->height > 200) {
        printf("Grid width and height cannot be above 200.\n");
        grid_dimensions = NULL;
        return;
    }

    grid_dimensions->cell_size = WINDOW_SIZE / grid_dimensions->width;
    grid_dimensions->is_valid = true;
}

int main(int argc, char* argv[])
{
    GridDimensions grid_dimensions = {0};

    get_grid_dimensions(argc, argv, &grid_dimensions);
    if (!grid_dimensions.is_valid)
        return -1;

    SDL_Window* window;
    SDL_Renderer* renderer;

    sdl_setup_window(&window, &renderer);
    if (window == NULL || renderer == NULL)
        return -1;

    run_simulation(&grid_dimensions, window, renderer);

    return sdl_cleanup(window, renderer);
}