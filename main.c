#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#define WIDTH            1200
#define HEIGHT           1000
#define BACKGROUND_COLOR 255, 255, 255, 255

#define MENU_WIDTH            WIDTH
#define MENU_HEIGHT           HEIGHT
#define MENU_BACKGROUND_COLOR 217, 217, 217, 255

#define COLOR_BLOCK_SIZE 40

typedef enum
{
    Rect = 0
} Shape;

// 2D array that contains all colors
unsigned char COLORS[][4] = {
    {255, 39, 39,  255},
    {182, 35, 35,  255},
    {100, 65, 13,  255},
    {178, 35, 182, 255}
};

// Number of rows in `COLORS` array
const size_t COLORS_N = sizeof(COLORS) / sizeof(COLORS[0]);

void render_menu(SDL_Renderer *ren, int selected_color)
{
    SDL_Rect background_rect = {
        .x = 0, .y = 0, .w = MENU_WIDTH, .h = MENU_HEIGHT
    };
    SDL_SetRenderDrawColor(ren, MENU_BACKGROUND_COLOR);
    SDL_RenderFillRect(ren, &background_rect);

    int padding = 10; // Padding 10px from the menu

    // TODO: Render the shapes
    // TODO: change shape on click

    // rendering colors section
    int colors_section_starting_x = padding;
    // TODO: start COLORS section y from shapes starting y + padding
    int colors_section_starting_y = 100;

    for (int i = 0; i < COLORS_N; ++i)
    {
        unsigned char r = COLORS[i][0];
        unsigned char g = COLORS[i][1];
        unsigned char b = COLORS[i][2];
        unsigned char a = COLORS[i][3];

        // Render selection outline
        if (selected_color == i)
        {
            SDL_Rect color_block_selection_outline_rect = {
                .x = colors_section_starting_x - 5,
                .y = colors_section_starting_y - 5,
                .w = COLOR_BLOCK_SIZE + 10,
                .h = COLOR_BLOCK_SIZE + 10
            };
            SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
            SDL_RenderFillRect(ren, &color_block_selection_outline_rect);
        }

        SDL_Rect color = {
            .x = colors_section_starting_x,
            .y = colors_section_starting_y,
            .w = COLOR_BLOCK_SIZE,
            .h = COLOR_BLOCK_SIZE
        };

        SDL_SetRenderDrawColor(ren, r, g, b, a);
        SDL_RenderFillRect(ren, &color);

        colors_section_starting_x += padding + COLOR_BLOCK_SIZE;

        // NOTE: `colors_section_starting_x + COLOR_BLOCK_SIZE` to prevent color
        // block from sticking to the edge of the menu
        if ((colors_section_starting_x + COLOR_BLOCK_SIZE) >= MENU_WIDTH)
        {
            colors_section_starting_x = padding;
            colors_section_starting_y += padding + COLOR_BLOCK_SIZE;
        }

        // TODO: change selected_color on click
    }
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
    bool display_menu    = true; // NOTE: set to 'true' for testing
    int selected_color   = 0;    // Default to use the first color
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

        // NOTE: The menu should be rendered last to appear on top
        if (display_menu == true)
            render_menu(ren, selected_color);

        SDL_RenderPresent(ren);
        // Render -----
    }

    TTF_CloseFont(font);
    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(ren);
    SDL_Quit();
}
