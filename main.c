#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>

#define WINDOW_WIDTH  900
#define WINDOW_HEIGHT 600
#define BORDER 8

SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        printf("Could not load image %s: %s\n", path, SDL_GetError());
        return NULL;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    return texture;
}

void drawFramedImage(SDL_Renderer* renderer, SDL_Texture* texture,
                     float x, float y, float w, float h) {
    SDL_FRect borderRect = { x - BORDER, y - BORDER, w + BORDER * 2, h + BORDER * 2 };
    SDL_SetRenderDrawColor(renderer, 218, 165, 32, 255);
    SDL_RenderFillRect(renderer, &borderRect);
    SDL_FRect imageRect = {x, y, w, h};
    SDL_RenderTexture(renderer, texture, NULL, &imageRect);
}

int main(int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window* window = SDL_CreateWindow("3D Rendering", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_Texture* wall   = loadTexture(renderer, "assets/scenes/wall.png");
    SDL_Texture* scene1 = loadTexture(renderer, "assets/scenes/Messy Room final.png");
    SDL_Texture* scene2 = loadTexture(renderer, "assets/scenes/Pedace After Pressure1.png");
    SDL_Texture* opengl = loadTexture(renderer, "assets/scenes/3D nonagon image.png");
    if (!wall || !scene1 || !scene2 || !opengl) {
        printf("One or more images failed to load.\n");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_Event event;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) running = false;
            if (event.type == SDL_EVENT_KEY_DOWN)
                if (event.key.key == SDLK_ESCAPE) running = false;
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_FRect fullWindow = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
        SDL_RenderTexture(renderer, wall, NULL, &fullWindow);
        drawFramedImage(renderer, scene1, 60, 80, 260, 200);
        drawFramedImage(renderer, scene2, 580, 80, 260, 280);
        drawFramedImage(renderer, opengl, 240, 330, 240, 200);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(wall);
    SDL_DestroyTexture(scene1);
    SDL_DestroyTexture(scene2);
    SDL_DestroyTexture(opengl);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}