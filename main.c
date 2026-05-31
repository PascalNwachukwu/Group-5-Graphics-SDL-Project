#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>

/* ── Window dimensions ── */
#define WINDOW_WIDTH  900
#define WINDOW_HEIGHT 600

/* ── Gold frame border thickness ── */
#define BORDER 8

/* ──────────────────────────────────────────
   Helper: load an image and return a texture
   Prints an error and returns NULL on failure
   ────────────────────────────────────────── */
SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        /* Tell us exactly which image failed and why */
        printf("Could not load image %s: %s\n", path, SDL_GetError());
        return NULL;
    }
    /* Convert the surface into a texture the renderer can draw */
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface); /* surface no longer needed once texture is made */
    return texture;
}

/* ──────────────────────────────────────────
   Helper: draw a gold-framed image on screen
   - renderer : the SDL renderer
   - texture  : the image to draw
   - x, y     : top-left position of the frame
   - w, h     : width and height of the image
   ────────────────────────────────────────── */
void drawFramedImage(SDL_Renderer* renderer, SDL_Texture* texture,
                     float x, float y, float w, float h) {

    /* 1. Draw the gold border rectangle behind the image */
    SDL_FRect borderRect = {
        x - BORDER,        /* start a little to the left   */
        y - BORDER,        /* start a little above          */
        w + BORDER * 2,    /* wider than the image          */
        h + BORDER * 2     /* taller than the image         */
    };
    SDL_SetRenderDrawColor(renderer, 218, 165, 32, 255); /* gold colour (R,G,B,A) */
    SDL_RenderFillRect(renderer, &borderRect);

    /* 2. Draw the actual image on top of the gold border */
    SDL_FRect imageRect = {x, y, w, h};
    SDL_RenderTexture(renderer, texture, NULL, &imageRect);
}

/* ══════════════════════════════════════════
   MAIN
   ══════════════════════════════════════════ */
int main(int argc, char* argv[]) {

    /* ── 1. Initialise SDL ── */
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    /* ── 2. Initialise SDL_image for PNG and JPG support ── */
    if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)) {
        printf("IMG_Init Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    /* ── 3. Create the window ── */
    SDL_Window* window = SDL_CreateWindow(
        "3D Rendering",   /* title bar text      */
        WINDOW_WIDTH,     /* width  in pixels    */
        WINDOW_HEIGHT,    /* height in pixels    */
        0                 /* no special flags    */
    );
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        IMG_Quit(); SDL_Quit();
        return 1;
    }

    /* ── 4. Create the renderer (the drawing tool) ── */
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit(); SDL_Quit();
        return 1;
    }

    /* ── 5. Load all images ── */
    SDL_Texture* wall   = loadTexture(renderer, "assets/scenes/wall.png");
    SDL_Texture* scene1 = loadTexture(renderer, "assets/scenes/scene1.png");
    SDL_Texture* scene2 = loadTexture(renderer, "assets/scenes/scene2.png");
    SDL_Texture* opengl = loadTexture(renderer, "assets/scenes/3D nonagon image.png");
    /* Stop if any image failed to load */
    if (!wall || !scene1 || !scene2 || !opengl) {
        printf("One or more images failed to load. Check assets/scenes/ folder.\n");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit(); SDL_Quit();
        return 1;
    }

    /* ── 6. Main event loop — keeps the window open ── */
    SDL_Event event;
    bool running = true;

    while (running) {

        /* Check for events (closing window or pressing ESC) */
        while (SDL_PollEvent(&event)) {

            /* User clicked the X button to close */
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }

            /* User pressed a key on the keyboard */
            if (event.type == SDL_EVENT_KEY_DOWN) {
                /* ESC key closes the window */
                if (event.key.key == SDLK_ESCAPE) {
                    running = false;
                }
            }
        }

        /* ── 7. Draw everything ── */

        /* Clear the screen to black first */
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        /* Draw the stone wall background stretched across the full window */
        SDL_FRect fullWindow = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
        SDL_RenderTexture(renderer, wall, NULL, &fullWindow);

        /* Draw scene1 (Blender scene 1) — top left
           Position: x=60,  y=80   Size: 260 x 200     */
        drawFramedImage(renderer, scene1, 60, 80, 260, 200);

        /* Draw scene2 (Blender scene 2) — top right
           Position: x=580, y=80   Size: 260 x 200     */
        drawFramedImage(renderer, scene2, 580, 80, 260, 200);

        /* Draw opengl scene — bottom centre
           Position: x=330, y=330  Size: 240 x 200     */
        drawFramedImage(renderer, opengl, 330, 330, 240, 200);

        /* Push everything to the screen */
        SDL_RenderPresent(renderer);
    }

    /* ── 8. Clean up everything before exiting ── */
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