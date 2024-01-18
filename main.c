#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "rendering.h"
#include "percolation.h"

// Todo: Add a "mode" CLI option that allows for random holes to be poked (maybe time the interval based on the size of the grid)
// Todo: Figure out the O notation lol

void get_grid_dimensions(int argc, char* argv[], GridDimensions* grid_dimensions) {
    if (argc - 1 < 1) {
        printf("Please provide a width and height for the percolation graph.\n");
        return;
    }

    grid_dimensions->virtual_size = strtol(argv[1], NULL, 10);

    if (grid_dimensions->virtual_size > 200) {
        printf("Grid width and height cannot be above 200.\n");
        grid_dimensions = NULL;
        return;
    }

    grid_dimensions->cell_size = (WINDOW_SIZE / grid_dimensions->virtual_size);
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

    // Infinite while loop, no exit after this line required
    run_simulation(&grid_dimensions, window, renderer);
}