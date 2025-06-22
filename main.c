#include <SDL2/SDL.h>

#define WIDTH 80
#define HEIGHT 40
#define CELL_SIZE 20

typedef struct State {
    SDL_Window *window;
    SDL_Renderer *renderer;
    int grid[HEIGHT][WIDTH];
    int running;
} state_t;

state_t state;

void render_grid() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            SDL_Rect cell = {x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
            if (state.grid[y][x])
                SDL_SetRenderDrawColor(state.renderer, 0, 150, 0, 255);
            else if ((x + y) % 2)
                SDL_SetRenderDrawColor(state.renderer, 20, 20, 20, 255);
            else
                SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
            SDL_RenderFillRect( state.renderer, &cell);
        }
    }
}

void update_grid() {
    int new_grid[HEIGHT][WIDTH] = {0};
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int neighbors = 0;
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    if (dx || dy) {
                        const int ny = y + dy;
                        const int nx = x + dx;
                        if (ny >= 0 && ny < HEIGHT && nx >= 0 && nx < WIDTH) {
                            neighbors += state.grid[ny][nx];
                        }
                    }
                }
            }
            new_grid[y][x] = neighbors == 3 || (state.grid[y][x] && neighbors == 2);
        }
    }

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            state.grid[y][x] = new_grid[y][x];
        }
    }
}

int main() {
    // Alloc ram and fill with 0
    memset(state.grid, 0, sizeof(state.grid));

    // Initialize with a simple pattern (glider)
    state.grid[1][2] =
        state.grid[2][3] =
            state.grid[3][1] =
                state.grid[3][2] =
                    state.grid[3][3] = 1;

    state.window = SDL_CreateWindow("GOL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE,SDL_WINDOW_SHOWN);
    state.renderer = SDL_CreateRenderer(state.window, -1, SDL_RENDERER_ACCELERATED);

    state.running = 1;
    while (state.running) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) state.running = 0;
        }

        render_grid();
        SDL_RenderPresent(state.renderer);
        SDL_Delay(200);
        update_grid();
    }

    // Clean up
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    SDL_Quit();

    return 0;
}

