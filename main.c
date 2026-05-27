#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#undef main

/* ── Window dimensions ── */
#define WINDOW_WIDTH  900
#define WINDOW_HEIGHT 600

/* ── Frame border thickness (the gold border) ── */
#define BORDER 8

/* ──────────────────────────────────────────
   Helper: load an image and return a texture
   Prints an error and returns NULL on failure
   ────────────────────────────────────────── */
SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        printf("Could not load image %s: %s\n", path, IMG_GetError());
        return NULL;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);  /* surface no longer needed once texture is made */
    return texture;
}

/* ──────────────────────────────────────────
   Helper: draw a framed image on the screen
   - renderer : the SDL renderer
   - texture  : the image to draw
   - x, y     : top-left position of the FRAME
   - w, h     : width and height of the IMAGE
   ────────────────────────────────────────── */
void drawFramedImage(SDL_Renderer* renderer, SDL_Texture* texture,
                     int x, int y, int w, int h) {

    /* 1. Draw the gold border (slightly bigger rectangle behind the image) */
    SDL_Rect borderRect = {
        x - BORDER,
        y - BORDER,
        w + BORDER * 2,
        h + BORDER * 2
    };
    SDL_SetRenderDrawColor(renderer, 218, 165, 32, 255); /* gold colour */
    SDL_RenderFillRect(renderer, &borderRect);

    /* 2. Draw the image on top of the border */
    SDL_Rect imageRect = {x, y, w, h};
    SDL_RenderCopy(renderer, texture, NULL, &imageRect);
}

/* ══════════════════════════════════════════
   MAIN
   ══════════════════════════════════════════ */
int main(int argc, char* argv[]) {

    /* ── 1. Initialise SDL and SDL_image ── */
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))) {
        printf("IMG_Init Error: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    /* ── 2. Create the window ── */
    SDL_Window* window = SDL_CreateWindow(
        "3D Rendering",           /* title bar text          */
        SDL_WINDOWPOS_CENTERED,   /* centre horizontally     */
        SDL_WINDOWPOS_CENTERED,   /* centre vertically       */
        WINDOW_WIDTH,             /* width  in pixels        */
        WINDOW_HEIGHT,            /* height in pixels        */
        SDL_WINDOW_SHOWN          /* make it visible immediately */
    );
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        IMG_Quit(); SDL_Quit();
        return 1;
    }

    /* ── 3. Create the renderer (the drawing tool) ── */
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED
    );
    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit(); SDL_Quit();
        return 1;
    }

    /* ── 4. Load all images ── */
    SDL_Texture* wall   = loadTexture(renderer, "assets/scenes/wall.png");
    SDL_Texture* scene1 = loadTexture(renderer, "assets/scenes/scene1.png");
    SDL_Texture* scene2 = loadTexture(renderer, "assets/scenes/scene2.png");
    SDL_Texture* opengl = loadTexture(renderer, "assets/scenes/opengl.png");

    /* Stop if any image failed to load */
    if (!wall || !scene1 || !scene2 || !opengl) {
        printf("One or more images failed to load. Check assets/scenes/ folder.\n");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit(); SDL_Quit();
        return 1;
    }

    /* ── 5. Main event loop — keeps window open ── */
    int running = 1;
    SDL_Event event;

    while (running) {

        /* Check for events (like clicking the X to close) */
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;  /* exit the loop */
            }
        }

        /* ── 6. Draw everything ── */

        /* Clear screen */
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        /* Draw wall background stretched across full window */
        SDL_Rect fullWindow = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
        SDL_RenderCopy(renderer, wall, NULL, &fullWindow);

        /* Draw scene1 — top left
           Position: x=60,  y=80   Size: 260 x 200 */
        drawFramedImage(renderer, scene1, 60, 80, 260, 200);

        /* Draw scene2 — top right
           Position: x=580, y=80   Size: 260 x 200 */
        drawFramedImage(renderer, scene2, 580, 80, 260, 200);

        /* Draw opengl scene — bottom centre
           Position: x=330, y=330  Size: 240 x 200 */
        drawFramedImage(renderer, opengl, 330, 330, 240, 200);

        /* Push everything to the screen */
        SDL_RenderPresent(renderer);
    }

    /* ── 7. Clean up everything before exiting ── */
    SDL_DestroyTexture(wall);
    SDL_DestroyTexture(scene1);
    SDL_DestroyTexture(scene2);
    SDL_DestroyTexture(opengl);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}