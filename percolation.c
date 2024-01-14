#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "percolation.h"
#include "rendering.h"

// These are virtual and will not be stored inside the union data structure
// These save time complexity by being assigned to the parent of the top and bottom grid elements
#define TOP_NODE_ID -1
#define BOTTOM_NODE_ID -2

typedef struct {
    SDL_Rect rect;
    int root;
    bool is_open;
} GridCell;

int get_hovered_cell_index(SDL_Rect* grid_cursor, GridDimensions* grid_dimensions) {
    // Todo: Maybe this calculation could be simplified by changing the values of grid_cursor?
    return (grid_cursor->x / grid_dimensions->cell_size) + ((grid_cursor->y / grid_dimensions->cell_size) * grid_dimensions->width);
}

void run_simulation(GridDimensions* grid_dimensions, SDL_Renderer* renderer) {
    // Place the grid cursor in the middle of the grid.
    SDL_Rect grid_cursor = {
        .x = (grid_dimensions->width - 1) / 2 * grid_dimensions->cell_size,
        .y = (grid_dimensions->height - 1) / 2 * grid_dimensions->cell_size,
        .w = grid_dimensions->cell_size,
        .h = grid_dimensions->cell_size,
    };

    bool quit = false;
    bool mouse_clicked = false;

    // Disjoint-set data structure
    const int grid_cell_count = grid_dimensions->width * grid_dimensions->height;
    GridCell* grid_cells = malloc(grid_cell_count * sizeof(GridCell));

    for (int i = 0; i < grid_cell_count; i++) {
        int root = i;

        if (i + 1 > grid_dimensions->width) {
            bool is_at_bottom_row = i + 1 > grid_dimensions->width * (grid_dimensions->height - 1);
            root = is_at_bottom_row ? BOTTOM_NODE_ID : TOP_NODE_ID;
        }

        GridCell* grid_cell = &grid_cells[i];
        grid_cell->root = root;
        grid_cell->is_open = false;

        // It's best not to ask
        int height_excess = i % grid_dimensions->height;
        grid_cell->rect = (SDL_Rect) {
           .x = height_excess * grid_dimensions->cell_size,
           .y = ((i - height_excess) / grid_dimensions->height) * grid_dimensions->cell_size,
           .w = grid_dimensions->cell_size,
           .h = grid_dimensions->cell_size,
        };
    }

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
    
        if (mouse_clicked) {
            int cell_index = get_hovered_cell_index(&grid_cursor, grid_dimensions);
            grid_cells[cell_index].is_open = true;
            // Todo: connect with surrounding cells using Disjoint-set
            // Todo: check if the Disjoint-set percolates
            // Todo: (if simple enough) highlight percolating area in blue
        }

        for (int i = 0; i < grid_cell_count; i++) {
            GridCell* grid_cell = &grid_cells[i];
            if (!grid_cell->is_open) continue;

            sdl_poke_hole(renderer, &grid_cell->rect);
        }

        // Todo: Optimise this so that it doesn't draw the grid every loop (just shrink the size of the grid_cursor by a pixel and render once) - is that even possible?
        sdl_draw_grid_outlines(grid_dimensions, renderer);

        SDL_RenderPresent(renderer);
        mouse_clicked = false;
    }
}