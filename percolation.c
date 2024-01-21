#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "percolation.h"
#include "rendering.h"

#define BORDER_SIZE 1
#define INVALID_CELL -1

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

void sleep_ms(int milliseconds)
{
    #ifdef _WIN32
        Sleep(milliseconds);
    #else
        usleep(milliseconds * 1000);
    #endif
}

typedef struct {
    SDL_Rect rect;
    int root;
    int size;
    bool is_open;
    Color cell_color;
} GridCell;

typedef struct {
    int x;
    int y;
} Coordinates;

int clamp(int num, int min, int max) {
    if (num > max) return max;
    if (num < min) return min;

    return num;
}

int get_hovered_cell_index(Coordinates* mouse_pos, int cell_size, int grid_virtual_size) {
    int cell_x = (mouse_pos->x - BORDER_SIZE) / cell_size;
    int cell_y = (mouse_pos->y - BORDER_SIZE) / cell_size;
    return (cell_y * grid_virtual_size) + cell_x;
}

int get_root(GridCell* grid_cells, int cid) {
    while (cid != grid_cells[cid].root) {
        // Flatten the tree for much faster searching
        GridCell* root_cell = &grid_cells[cid];
        root_cell->root = grid_cells[root_cell->root].root;

        cid = root_cell->root;
    }

    return cid;
}

bool is_connected(GridCell* grid_cells, int cid_a, int cid_b) {
    return get_root(grid_cells, cid_a) == get_root(grid_cells, cid_b);
}

void connect_nodes(GridCell* grid_cells, int cid_a, int cid_b) {
    int a_root = get_root(grid_cells, cid_a);
    int b_root = get_root(grid_cells, cid_b);

    if (a_root == b_root) return;

    GridCell* a_root_cell = &grid_cells[a_root];
    GridCell* b_root_cell = &grid_cells[b_root];

    // Place larger tree above smaller tree for faster searching
    if (a_root_cell->size > b_root_cell->size) {
        b_root_cell->root = a_root_cell->root;
        a_root_cell->size += b_root_cell->size;
        return;
    }

    a_root_cell->root = b_root_cell->root;
    b_root_cell->size += a_root_cell->size;
}

bool is_index_top_row(int index, int grid_width) {
    return index < grid_width - 1;
}

bool is_index_bottom_row(int index, int grid_width, int grid_cell_count) {
    return index > (grid_cell_count - grid_width) - 1;
}

void populate_disjoint_set(GridCell* grid_cells, int cell_count, int top_node_id, int bottom_node_id, GridDimensions* grid_dimensions) {
    for (int i = 0; i < cell_count; i++) {
        int root = i;

        if (is_index_top_row(i, grid_dimensions->virtual_size))
            root = top_node_id;
        else if (is_index_bottom_row(i, grid_dimensions->virtual_size, cell_count))
            root = bottom_node_id;

        GridCell grid_cell = {0};
        grid_cell.root = root;

        // It's best not to ask
        int height_excess = i % grid_dimensions->virtual_size;
        grid_cell.rect = (SDL_Rect) {
           .x = (height_excess * grid_dimensions->cell_size) + BORDER_SIZE,
           .y = (((i - height_excess) / grid_dimensions->virtual_size) * grid_dimensions->cell_size) + BORDER_SIZE,
           .w = grid_dimensions->cell_size - BORDER_SIZE,
           .h = grid_dimensions->cell_size - BORDER_SIZE
        };

        grid_cell.size = 1;
        grid_cells[i] = grid_cell;
    }

    // Initialise virtual nodes
    grid_cells[top_node_id] = (GridCell) {.root = top_node_id, .rect = {0}};
    grid_cells[bottom_node_id] = (GridCell) {.root = bottom_node_id, .rect = {0}};
}

void connect_neighbour(int neighbour_cell_index, int hovered_cell_index, GridCell* grid_cells) {
    if (neighbour_cell_index != INVALID_CELL && grid_cells[neighbour_cell_index].is_open)
        connect_nodes(grid_cells, neighbour_cell_index, hovered_cell_index);
}

// This is neccesary because grids above size 100 are going to exceed RAND_MAX
int large_rand() {
    return (rand() * RAND_MAX + rand());
}

void run_simulation(Mode mode, int random_interval_mills, GridDimensions* grid_dimensions, SDL_Window* window, SDL_Renderer* renderer) {
    Coordinates mouse_pos = { .x=0, .y=0 };

    // Disjoint-set data structure
    const int grid_cell_count = grid_dimensions->virtual_size * grid_dimensions->virtual_size;

    // The last two elements are the virtual nodes at the top and bottom of the disjoint-set tree
    GridCell* grid_cells = malloc((grid_cell_count + 2) * sizeof(GridCell));

    int top_node_id = grid_cell_count;
    int bottom_node_id = grid_cell_count + 1;

    populate_disjoint_set(
        grid_cells,
        grid_cell_count,
        top_node_id,
        bottom_node_id,
        grid_dimensions
    );


    bool mouse_down = false;
    bool has_percolated = false;

    // Initialise RNG
    srand(time(0));

    while (true) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_MOUSEMOTION:
                    mouse_pos.x = clamp(event.motion.x, 0, WINDOW_SIZE);
                    mouse_pos.y = clamp(event.motion.y, 0, WINDOW_SIZE);

                    break;
                case SDL_MOUSEBUTTONDOWN:
                    mouse_down = true;
                    break;
                case SDL_MOUSEBUTTONUP:
                    mouse_down = false;
                    break;
                case SDL_QUIT:
                    sdl_cleanup(window, renderer);
                    free(grid_cells);
                    exit(0);
                    break;
            }
        }

        if (has_percolated) continue;

        int cell_index;

        switch(mode) {
            case Random:
                cell_index = large_rand() % grid_cell_count;
                while (grid_cells[cell_index].is_open) {
                    cell_index = large_rand() % grid_cell_count;
                }

                if (random_interval_mills > 0)
                    sleep_ms(random_interval_mills);

                break;
            case User:
                if (!mouse_down) continue;
                cell_index = get_hovered_cell_index(&mouse_pos, grid_dimensions->cell_size, grid_dimensions->virtual_size);
                break;
        }
        
        GridCell* grid_cell = &grid_cells[cell_index];
        grid_cell->is_open = true;

        // This pains me
        bool is_top_row = is_index_top_row(cell_index, grid_dimensions->virtual_size);
        bool is_bottom_row = is_index_bottom_row(cell_index, grid_dimensions->virtual_size, grid_cell_count);
        bool is_left_col = cell_index % grid_dimensions->virtual_size == 0;
        bool is_right_col = (cell_index + 1) % grid_dimensions->virtual_size == 0;

        const int INVALID = -1;

        int left_cell = is_left_col ? INVALID : cell_index - 1;
        int right_cell = is_right_col ? INVALID : cell_index + 1;
        int top_cell = is_top_row ? INVALID : cell_index - grid_dimensions->virtual_size;
        int bottom_cell = is_bottom_row ? INVALID : cell_index + grid_dimensions->virtual_size;

        connect_neighbour(left_cell, cell_index, grid_cells);
        connect_neighbour(right_cell, cell_index, grid_cells);
        connect_neighbour(top_cell, cell_index, grid_cells);
        connect_neighbour(bottom_cell, cell_index, grid_cells);

        Color hole_color = is_connected(grid_cells, cell_index, top_node_id) ? Blue : White;
        grid_cell->cell_color = hole_color;
        sdl_poke_hole(renderer, &grid_cell->rect, hole_color);

        // Color percolating cells
        // Yucky time complexity, but there isn't really a choice
        for (int i = 0; i < grid_cell_count; i++) {
            GridCell* grid_cell = &grid_cells[i];
            if (grid_cell->cell_color == Blue) continue;

            if (is_connected(grid_cells, i, top_node_id) && grid_cell->is_open) {
                sdl_poke_hole(renderer, &grid_cell->rect, Blue);
                grid_cell->cell_color = Blue;
            }
        }

        SDL_RenderPresent(renderer);

        has_percolated = is_connected(grid_cells, top_node_id, bottom_node_id);
        if (has_percolated) {
            sdl_messagebox_warning(window, "The grid now percolates.");
            mouse_down = false;
        }
    }
}