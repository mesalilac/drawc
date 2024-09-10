#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#define WIDTH            1200
#define HEIGHT           1000
#define BACKGROUND_COLOR 255, 255, 255, 255

typedef enum
{
    Rect = 0
} Shape;

// 2D array that contains all colors
unsigned char COLORS[][4] = {
    {255, 39, 39, 255},
    {100, 65, 13, 255}
};

// Number of rows in `COLORS` array
const size_t COLORS_N = sizeof(COLORS) / sizeof(COLORS[0]);

void render_menu(SDL_Renderer *ren)
{
    // TODO: Render the shapes and colors
    // TODO: Click on Shapes or colors and change state
}

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(
            stderr, "ERROR: Failed to initalize SDL VIDEO: %s", SDL_GetError()
        );
        exit(1);
    }

    SDL_Window *win =
        SDL_CreateWindow("Drawc", 0, 0, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (win == NULL)
    {
        fprintf(stderr, "ERROR: Failed to create window: %s", SDL_GetError());
        exit(1);
    }

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (ren == NULL)
    {
        fprintf(stderr, "ERROR: Failed to create renderer: %s", SDL_GetError());
        exit(1);
    }

    if (TTF_Init() == -1)
    {
        fprintf(
            stderr, "ERROR: Failed to initalize sdl ttf: %s", SDL_GetError()
        );
        exit(1);
    }

    TTF_Font *font = TTF_OpenFont("./yudit.ttf", 18);
    if (font == NULL)
    {
        fprintf(stderr, "ERROR: Failed to open font: %s", SDL_GetError());
        exit(1);
    }

    bool is_running      = true;
    bool display_menu    = false;
    int selected_color   = 0; // Default to use the first color
    Shape selected_shape = Rect;
    SDL_Event event;

    while (is_running)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    is_running = false;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == 'm') // Open menu
                        display_menu = !display_menu;
                    break;
            }
        }

        // ----- Render
        SDL_SetRenderDrawColor(ren, BACKGROUND_COLOR);
        SDL_RenderClear(ren);

        SDL_Rect rect = {.x = 0, .y = 0, .w = 20, .h = 20};
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderFillRect(ren, &rect);

        // Render menu
        // NOTE: The menu should be rendered last to appear on top
        if (display_menu == true)
            render_menu(ren);

        SDL_RenderPresent(ren);
        // Render -----
    }

    TTF_CloseFont(font);
    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(ren);
    SDL_Quit();
}
