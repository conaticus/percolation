#include <SDL2/SDL.h>

// Values
const int GRID_CELL_SIZE = 50;

// Theme
const SDL_Color COLOR_WHITE = { 255, 255, 255, 255 };
const SDL_Color COLOR_BLACK = { 0, 0, 0, 255 };
const SDL_Color COLOR_BLUE = { 115, 169, 255, 255 };

int sdl_cleanup(SDL_Window* window) {
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

int main(int argc, char* argv[])
{
    if (argc - 1 < 2) {
        printf("Please provide a width and height for the percolation graph.\n");
        return 1;
    }

    int grid_width = SDL_strtol(argv[1], NULL, 10);
    int grid_height = SDL_strtol(argv[2], NULL, 10);

    // + 1 so that the last grid lines fit in the screen
    int window_width = (grid_width * GRID_CELL_SIZE) + 1;
    int window_height = (grid_height * GRID_CELL_SIZE) + 1;

    // Place the grid cursor in the middle of the screen.
    SDL_Rect grid_cursor = {
        .x = (grid_width - 1) / 2 * GRID_CELL_SIZE,
        .y = (grid_height - 1) / 2 * GRID_CELL_SIZE,
        .w = GRID_CELL_SIZE,
        .h = GRID_CELL_SIZE,
    };

    SDL_Rect* highlighted_squares = SDL_malloc(grid_width * grid_height * sizeof(SDL_Rect));
    int highlighted_square_count = 0;

    // Todo: Failable
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window;
    SDL_Renderer *renderer;

    // Todo: Failable
    SDL_CreateWindowAndRenderer(window_width, window_height, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Percolation");

    if (window == NULL) {
        printf("Could not create window %s\n", SDL_GetError());
        return -1;
    }

    SDL_bool quit = SDL_FALSE;
    SDL_bool mouse_clicked = SDL_FALSE;

    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_MOUSEMOTION:
                    grid_cursor.x = (event.motion.x / GRID_CELL_SIZE) * GRID_CELL_SIZE;
                    grid_cursor.y = (event.motion.y / GRID_CELL_SIZE) * GRID_CELL_SIZE;

                    break;
                case SDL_MOUSEBUTTONDOWN:
                    mouse_clicked = SDL_TRUE;
                    break;
                case SDL_QUIT:
                    quit = SDL_TRUE;
                    break;
            }
        }


        if (mouse_clicked)
            highlighted_squares[highlighted_square_count++] = grid_cursor;

        for (int i = 0; i < highlighted_square_count; i++) {
            // Todo: Check if rect is already in the array (should probably initialise a static array where they are either on or off)

            SDL_SetRenderDrawColor(renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, COLOR_WHITE.a);
            SDL_RenderFillRect(renderer, &highlighted_squares[i]);
        }

        // Todo: Optimise this so that it doesn't draw the grid every loop (just shrink the size of the grid_cursor by a pixel and render once)

        // Draw grid lines vertical
        SDL_SetRenderDrawColor(renderer, COLOR_BLACK.r, COLOR_BLACK.g, COLOR_BLACK.b, COLOR_BLACK.a);
        for (int x = 0; x < 1 + grid_width * GRID_CELL_SIZE; x += GRID_CELL_SIZE) {
            SDL_RenderDrawLine(renderer, x, 0, x, window_height);
        }

        // Draw grid line horizontal
        for (int y = 0; y < 1 + grid_height * GRID_CELL_SIZE; y += GRID_CELL_SIZE) {
            SDL_RenderDrawLine(renderer, 0, y, window_width, y);
        }

        SDL_RenderPresent(renderer);
        mouse_clicked = SDL_FALSE;
    }

    SDL_free(highlighted_squares);
    return sdl_cleanup(window);
}