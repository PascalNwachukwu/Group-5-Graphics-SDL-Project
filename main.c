#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>

#define BORDER 12

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
   SDL_Window* window = SDL_CreateWindow("3D Rendering", 1280, 720, SDL_WINDOW_MAXIMIZED | SDL_WINDOW_FULLSCREEN);
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
    int W, H;
    SDL_GetWindowSize(window, &W, &H);
    float sw = (float)W;
    float sh = (float)H;
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
        SDL_FRect fullWindow = {0, 0, sw, sh};
        SDL_RenderTexture(renderer, wall, NULL, &fullWindow);
        float imgW = sw * 0.30f;
        float imgH = sh * 0.42f;
       drawFramedImage(renderer, scene1, sw * 0.05f, sh * 0.06f, imgW, imgH);
drawFramedImage(renderer, scene2, sw * 0.65f, sh * 0.06f, imgW, imgH);
drawFramedImage(renderer, opengl, sw * 0.35f, sh * 0.52f, imgW, imgH);
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