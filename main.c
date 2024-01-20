#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "rendering.h"
#include "percolation.h"

void get_args(int argc, char* argv[], int* random_interval, Mode* mode, GridDimensions* grid_dimensions) {
    if (argc - 1 < 2) {
        printf("Please provide a grid size for the percolation graph as well as a mode ('user' | 'random') e.g 'bin user [grid size]' or 'bin random [interval] [grid size]'.\n");
        return;
    }

    char* chosen_mode = argv[1];
    int next_arg_id = 2;

    if (strcmp(chosen_mode, "user") == 0) *mode = User;
    else if (strcmp(chosen_mode, "random") == 0) {

        if (argc - 1 < 3) {
            printf("For random mode, please specify a time interval in millseconds, e.g: 'bin random [interval millseconds] [grid size]'\n");
            return;
        }

        next_arg_id++;
        *mode = Random;
        *random_interval = strtol(argv[2], NULL, 10);
    } else {
        printf("Please provide a valid mode - 'user' or 'random' (case sensitive)\n");
        return;
    }

    grid_dimensions->virtual_size = strtol(argv[next_arg_id], NULL, 10);

    if (grid_dimensions->virtual_size > 100) {
        printf("Grid width and height cannot be above 100.\n");
        grid_dimensions = NULL;
        return;
    }

    grid_dimensions->cell_size = (WINDOW_SIZE / grid_dimensions->virtual_size);
    grid_dimensions->is_valid = true;
}

int main(int argc, char* argv[])
{
    GridDimensions grid_dimensions = {0};

    Mode mode = -1;
    int random_interval = -1;

    get_args(argc, argv, &random_interval, &mode, &grid_dimensions);
    if (!grid_dimensions.is_valid || mode == -1)
        return 1;

    if (mode == Random && random_interval == -1)
        return 1;

    SDL_Window* window;
    SDL_Renderer* renderer;

    sdl_setup_window(&window, &renderer);
    if (window == NULL || renderer == NULL)
        return 1;

    // Infinite while loop, no exit after this line required
    run_simulation(mode, random_interval, &grid_dimensions, window, renderer);
}