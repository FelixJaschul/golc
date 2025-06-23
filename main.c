#include <stdbool.h>
#include <SDL2/SDL.h>

#define WIDTH 80
#define HEIGHT 40
#define CELL_SIZE 20

typedef struct State {
    SDL_Window *window;
    SDL_Renderer *renderer;
    int grid[HEIGHT][WIDTH];
    bool running;
    bool paused;

    struct Mouse {
        int x, y;
    } mouse;
} state_t;

state_t state;

int getNeighbors(const int y, const int x) {
    int neighbors = 0;

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx != 0 || dy != 0) {
                const int ny = y + dy;
                const int nx = x + dx;
                if (ny >= 0
                        && ny < HEIGHT
                        && nx >= 0
                        && nx < WIDTH) {
                    neighbors += state.grid[ny][nx];
                }
            }
        }
    }
    return neighbors;
}

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
    int new_grid[HEIGHT][WIDTH];
    memset(new_grid, 0, sizeof(new_grid));

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            const int neighbors = getNeighbors(y, x);

            // Apply Conway's Game of Life rules
            // 1. Any live cell with 2 or 3 live neighbors survives
            // 2. Any dead cell with exactly 3 live neighbors becomes alive
            // 3. All other cells die or stay dead
            new_grid[y][x] = neighbors == 3 || (state.grid[y][x] && neighbors == 2);
        }
    }

    // Update mouse pos
    int mx, my;
    SDL_GetMouseState(&mx, &my);
    state.mouse.y = my / CELL_SIZE;
    state.mouse.x = mx / CELL_SIZE;

    // Paste updated grid into viewable grid
    if (state.paused) return;
    for (int y = 0; y < HEIGHT; y++) {
        memcpy(state.grid[y], new_grid[y], WIDTH * sizeof(int));
    }
}

void init() {
    memset(state.grid, 0, sizeof(state.grid));
    state.window = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE,SDL_WINDOW_SHOWN);
    state.renderer = SDL_CreateRenderer(state.window, -1, SDL_RENDERER_ACCELERATED);
    state.running = true;
    state.paused = false;
}

void deinit() {
    // Clean up
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    SDL_Quit();
}

int main() {
    init();
    // Initialize with a simple pattern (glider)
    state.grid[1][2] =
        state.grid[2][3] =
            state.grid[3][1] =
                state.grid[3][2] =
                    state.grid[3][3] = 1;

    while (state.running) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) state.running = false;
            if (ev.type == SDL_MOUSEBUTTONDOWN) state.grid[state.mouse.y][state.mouse.x] = 1;
            if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_SPACE) {state.paused = !state.paused; printf("PAUSED"); };
        }

        render_grid();
        SDL_RenderPresent(state.renderer);
        SDL_Delay(200);
        update_grid();

        printf("MOUSE POS: %d / %d \n", state.mouse.x, state.mouse.y);
    }
    deinit();
    return 0;
}
