#ifndef PERCOLATION_H
#define PERCOLATION_H

#include <SDL2/SDL.h>
#include "rendering.h"

void run_simulation(GridDimensions* grid_dimensions, SDL_Window* window, SDL_Renderer* renderer);

#endif // PERCOLATION_H