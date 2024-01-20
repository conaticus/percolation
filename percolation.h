#ifndef PERCOLATION_H
#define PERCOLATION_H

#include <SDL2/SDL.h>
#include "rendering.h"

typedef enum {
    User,
    Random,
} Mode;

void run_simulation(Mode mode, int random_interval_mills, GridDimensions* grid_dimensions, SDL_Window* window, SDL_Renderer* renderer);

#endif // PERCOLATION_H