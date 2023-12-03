#include "render.hpp"

Render::Render() {
    initWindow();
    loadTilemap();
    SDL_SetRelativeMouseMode(SDL_TRUE);
    raycaster = make_shared<Raycaster>();
}

Render::~Render() {
    window ? SDL_DestroyWindow(window) : void(0);
    renderer ? SDL_DestroyRenderer(renderer) : void(0);
    worldMapTexture ? SDL_DestroyTexture(worldMapTexture) : void(0);
    worldMapSurface ? SDL_FreeSurface(worldMapSurface) : void(0);
    SDL_SetRelativeMouseMode(SDL_FALSE);
    SDL_Quit();
}

void Render::draw() {
    drawPlayerVision();
    drawMiniMap();
}

void Render::run() {
    while (!quit) {
        handleEvents();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        updateMovements();
        draw();

        SDL_Delay(FPS);
        SDL_RenderPresent(renderer);
    }
    SDL_Quit();
}
