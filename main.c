#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH            1200
#define HEIGHT           1000
#define BACKGROUND_COLOR 255, 255, 255, 255

#define PADDING 10

#define MENU_WIDTH            WIDTH
#define MENU_HEIGHT           HEIGHT
#define MENU_BACKGROUND_COLOR 217, 217, 217, 255

#define COLOR_BLOCK_SIZE 40

// 2D array that contains all colors
unsigned char COLORS[][4] = {
    {246, 24,  40,  255},
    {238, 234, 26,  255},
    {238, 150, 26,  255},
    {83,  229, 29,  255},
    {29,  150, 229, 255},
    {29,  79,  229, 255},
    {150, 29,  229, 255},
    {229, 29,  217, 255},
    {229, 29,  150, 255},
};

const int COLORS_N = sizeof(COLORS) / sizeof(COLORS[0]);

#define SHAPE_RECT 0

int SHAPES[] = {SHAPE_RECT};

const int SHAPES_N = sizeof(SHAPES) / sizeof(SHAPES[0]);

typedef struct
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} Color;

typedef struct
{
    SDL_Rect rect;
    Color color;
} Shape;

void render_menu(SDL_Renderer *ren, int *selected_shape, int *selected_color)
{
    int mouse_x, mouse_y;
    Uint32 buttons   = SDL_GetMouseState(&mouse_x, &mouse_y);
    SDL_Point cursor = {mouse_x, mouse_y};

    SDL_Rect background_rect = {
        .x = 0, .y = 0, .w = MENU_WIDTH, .h = MENU_HEIGHT
    };
    SDL_SetRenderDrawColor(ren, MENU_BACKGROUND_COLOR);
    SDL_RenderFillRect(ren, &background_rect);

    int shapes_x = PADDING;
    int shapes_y = PADDING;

    for (int i = 0; i < SHAPES_N; ++i)
    {
        switch (i)
        {
            case SHAPE_RECT:
                if (*selected_shape == SHAPE_RECT)
                {
                    SDL_Rect selection_outline_rect = {
                        .x = shapes_x - 5,
                        .y = shapes_y - 5,
                        .w = COLOR_BLOCK_SIZE + 10,
                        .h = COLOR_BLOCK_SIZE + 10
                    };
                    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
                    SDL_RenderFillRect(ren, &selection_outline_rect);
                }

                SDL_Rect rect_shape = {
                    .x = shapes_x,
                    .y = shapes_y,
                    .w = COLOR_BLOCK_SIZE,
                    .h = COLOR_BLOCK_SIZE
                };
                SDL_SetRenderDrawColor(ren, 200, 80, 80, 255);
                SDL_RenderFillRect(ren, &rect_shape);

                if (SDL_PointInRect(&cursor, &rect_shape) &&
                    (buttons & SDL_BUTTON_LMASK) != 0)
                {
                    *selected_shape = SHAPE_RECT;
                }
                break;
        }

        shapes_x += PADDING + COLOR_BLOCK_SIZE;

        if ((shapes_x + COLOR_BLOCK_SIZE) >= MENU_WIDTH)
        {
            shapes_x = PADDING;
            shapes_y += PADDING + COLOR_BLOCK_SIZE;
        }
    }

    int colors_x = PADDING;
    int colors_y = shapes_y + COLOR_BLOCK_SIZE * 2 + PADDING;

    for (int i = 0; i < COLORS_N; ++i)
    {
        unsigned char r = COLORS[i][0];
        unsigned char g = COLORS[i][1];
        unsigned char b = COLORS[i][2];
        unsigned char a = COLORS[i][3];

        // Render selection outline
        if (*selected_color == i)
        {
            SDL_Rect selection_outline_rect = {
                .x = colors_x - 5,
                .y = colors_y - 5,
                .w = COLOR_BLOCK_SIZE + 10,
                .h = COLOR_BLOCK_SIZE + 10
            };
            SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
            SDL_RenderFillRect(ren, &selection_outline_rect);
        }

        SDL_Rect color = {
            .x = colors_x,
            .y = colors_y,
            .w = COLOR_BLOCK_SIZE,
            .h = COLOR_BLOCK_SIZE
        };

        SDL_SetRenderDrawColor(ren, r, g, b, a);
        SDL_RenderFillRect(ren, &color);

        if (SDL_PointInRect(&cursor, &color) &&
            (buttons & SDL_BUTTON_LMASK) != 0)
        {
            *selected_color = i;
        }

        colors_x += PADDING + COLOR_BLOCK_SIZE;

        if ((colors_x + COLOR_BLOCK_SIZE) >= MENU_WIDTH)
        {
            colors_x = PADDING;
            colors_y += PADDING + COLOR_BLOCK_SIZE;
        }
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

    bool is_running    = true;
    bool display_menu  = false;
    int selected_color = 0;
    int selected_shape = SHAPE_RECT;
    SDL_Event event;

    Shape **shapes_list   = malloc(300 * sizeof(Shape));
    int shapes_list_count = 0;

    while (is_running)
    {
        int mouse_x, mouse_y;
        Uint32 buttons   = SDL_GetMouseState(&mouse_x, &mouse_y);
        SDL_Point cursor = {mouse_x, mouse_y};

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
                case SDL_MOUSEBUTTONDOWN:
                    if (display_menu == false)
                    {

                        SDL_Rect rect = {
                            .x = mouse_x - COLOR_BLOCK_SIZE / 2,
                            .y = mouse_y - COLOR_BLOCK_SIZE / 2,
                            .w = COLOR_BLOCK_SIZE,
                            .h = COLOR_BLOCK_SIZE,
                        };

                        Color color = {
                            .r = COLORS[selected_color][0],
                            .g = COLORS[selected_color][1],
                            .b = COLORS[selected_color][2],
                            .a = COLORS[selected_color][3]
                        };

                        Shape *shape = malloc(sizeof(Shape));
                        shape->rect  = rect;
                        shape->color = color;

                        shapes_list[shapes_list_count] = shape;
                        shapes_list_count += 1;

                        printf("x: %i, y: %i\n", mouse_x, mouse_y);
                    }
                    break;
            }
        }

        SDL_SetRenderDrawColor(ren, BACKGROUND_COLOR);
        SDL_RenderClear(ren);

        // NOTE: The menu should be rendered last to appear on top
        if (display_menu == true)
            render_menu(ren, &selected_shape, &selected_color);
        else
        {
            for (int i = 0; i < shapes_list_count; ++i)
            {
                Shape *shape = shapes_list[i];
                SDL_SetRenderDrawColor(
                    ren,
                    shape->color.r,
                    shape->color.g,
                    shape->color.b,
                    shape->color.a
                );
                SDL_RenderFillRect(ren, &shape->rect);
            }
            SDL_Rect outline = {
                .x = mouse_x - COLOR_BLOCK_SIZE / 2,
                .y = mouse_y - COLOR_BLOCK_SIZE / 2,
                .w = COLOR_BLOCK_SIZE,
                .h = COLOR_BLOCK_SIZE,
            };

            SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
            SDL_RenderDrawRect(ren, &outline);
        }

        SDL_RenderPresent(ren);
    }

    for (int i = 0; i < shapes_list_count; ++i)
    {
        free(shapes_list[i]);
    }

    free(shapes_list);

    TTF_CloseFont(font);
    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(ren);
    SDL_Quit();
}
