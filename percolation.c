#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "percolation.h"
#include "rendering.h"

// These are virtual and will not be stored inside the union data structure
// These save time complexity by being assigned to the parent of the top and bottom grid elements
// #define TOP_NODE_ROOT -1
// #define BOTTOM_NODE_ROOT -2

#define BORDER_SIZE 1

typedef struct {
    SDL_Rect rect;
    int root;
    bool is_open;
} GridCell;

int get_hovered_cell_index(SDL_Rect* grid_cursor, GridDimensions* grid_dimensions) {
    // Todo: Maybe this calculation could be simplified by changing the values of grid_cursor?
    return (grid_cursor->x / grid_dimensions->cell_size) + ((grid_cursor->y / grid_dimensions->cell_size) * grid_dimensions->width);
}

int get_root(GridCell* grid_cells, int cid) {
    while (cid != grid_cells[cid].root)
        cid = grid_cells[cid].root;

    return cid;
}

bool is_connected(GridCell* grid_cells, int cid_a, int cid_b) {
    return get_root(grid_cells, cid_a) == get_root(grid_cells, cid_b);
}

void connect(GridCell* grid_cells, int cid_a, int cid_b) {
    int a_root_id = get_root(grid_cells, cid_a); 
    grid_cells[a_root_id].root = get_root(grid_cells, cid_b);
}

bool is_index_top_row(int index, int grid_width) {
    return index < grid_width - 1;
}

bool is_index_bottom_row(int index, int grid_width, int grid_cell_count) {
    return index > (grid_cell_count - grid_width) - 1;
}

void run_simulation(GridDimensions* grid_dimensions, SDL_Window* window, SDL_Renderer* renderer) {
    // Place the grid cursor in the middle of the grid.
    SDL_Rect grid_cursor = {
        .x = (grid_dimensions->width - BORDER_SIZE) / 2 * grid_dimensions->cell_size,
        .y = (grid_dimensions->height - BORDER_SIZE) / 2 * grid_dimensions->cell_size,
        .w = grid_dimensions->cell_size,
        .h = grid_dimensions->cell_size,
    };

    // Disjoint-set data structure
    const int grid_cell_count = grid_dimensions->width * grid_dimensions->height;

    // The last two elements are the virtual nodes at the top and bottom of the grid tree
    GridCell* grid_cells = malloc(((grid_cell_count + 2) * sizeof(GridCell)));

    int top_node_id = grid_cell_count;
    int bottom_node_id = grid_cell_count + 1;

    for (int i = 0; i < grid_cell_count; i++) {
        int root = i;

        // Todo: Might need an offset
        if (is_index_top_row(i, grid_dimensions->width))
            root = top_node_id;
        else if (is_index_bottom_row(i, grid_dimensions->width, grid_cell_count))
            root = bottom_node_id;

        GridCell grid_cell = {0};
        grid_cell.root = root;

        // It's best not to ask
        int height_excess = i % grid_dimensions->height;
        grid_cell.rect = (SDL_Rect) {
           .x = height_excess * grid_dimensions->cell_size,
           .y = (((i - height_excess) / grid_dimensions->height) * grid_dimensions->cell_size),
           .w = grid_dimensions->cell_size - BORDER_SIZE,
           .h = grid_dimensions->cell_size - BORDER_SIZE,
        };

        grid_cells[i] = grid_cell;
    }

    grid_cells[top_node_id] = (GridCell) {.root = top_node_id, .rect = {0}};
    grid_cells[bottom_node_id] = (GridCell) {.root = bottom_node_id, .rect = {0}};

    sdl_draw_grid_outlines(grid_dimensions, renderer);

    bool quit = false;
    bool mouse_down = false;
    bool percolate_message_shown = false;

    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_MOUSEMOTION:
                    // Todo: Fix bug where percolates if mouse goes to bottom edge of screen - likely to do with the grid outlines
                    grid_cursor.x = (event.motion.x / grid_dimensions->cell_size) * grid_dimensions->cell_size;
                    grid_cursor.y = (event.motion.y / grid_dimensions->cell_size) * grid_dimensions->cell_size;

                    break;
                case SDL_MOUSEBUTTONDOWN:
                    mouse_down = true;
                    break;
                case SDL_MOUSEBUTTONUP:
                    mouse_down = false;
                    break;
                case SDL_QUIT:
                    quit = true;
                    break;
            }
        }
    
        // Todo: This is cursed and I'm going to fix it don't worry
        if (mouse_down) {
            int cell_index = get_hovered_cell_index(&grid_cursor, grid_dimensions);
            GridCell* grid_cell = &grid_cells[cell_index];
            grid_cell->is_open = true;
            sdl_poke_hole(renderer, &grid_cell->rect);
            SDL_RenderPresent(renderer);

            if (!percolate_message_shown) {
                int is_top_row = is_index_top_row(cell_index, grid_dimensions->width);
                int is_bottom_row = is_index_bottom_row(cell_index, grid_dimensions->width, grid_cell_count);
                int is_left_col = cell_index % grid_dimensions->width == 0;
                int is_right_col = (cell_index + 1) % grid_dimensions->width == 0;
                
                const int UNDEFINED_INDEX = -1;

                int left = is_left_col ? UNDEFINED_INDEX : cell_index - 1;
                int right = is_right_col ? UNDEFINED_INDEX : cell_index + 1;
                int top = is_top_row ? UNDEFINED_INDEX : cell_index - grid_dimensions->width;
                int bottom = is_bottom_row ? UNDEFINED_INDEX : cell_index + grid_dimensions->width;

                if (left != UNDEFINED_INDEX && grid_cells[left].is_open)
                    connect(grid_cells, left, cell_index);

                if (right != UNDEFINED_INDEX && grid_cells[right].is_open)
                    connect(grid_cells, right, cell_index);

                if (top != UNDEFINED_INDEX && grid_cells[top].is_open)
                    connect(grid_cells, top, cell_index);

                if (bottom != UNDEFINED_INDEX && grid_cells[bottom].is_open)
                    connect(grid_cells, bottom, cell_index);

                bool percolates = is_connected(grid_cells, top_node_id, bottom_node_id);
                if (percolates) {
                    sdl_messagebox_warning(window, "The grid now percolates.");
                    SDL_RenderPresent(renderer);
                    percolate_message_shown = true;
                    mouse_down = false;
                }

                // Todo: (if simple enough) highlight percolating area in blue
            }
        }
    }

    free(grid_cells);
}