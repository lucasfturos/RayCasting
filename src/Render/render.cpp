#include "render.hpp"
using std::make_shared;

Render::Render() : raycaster(make_shared<Raycaster>()) {
    initWindow();
    initTexture();
    SDL_SetRelativeMouseMode(SDL_TRUE);
}

Render::~Render() {
    if (window)
        SDL_DestroyWindow(window);
    if (renderer)
        SDL_DestroyRenderer(renderer);

    if (worldMapSurface)
        SDL_FreeSurface(worldMapSurface);
    if (worldMapTexture)
        SDL_DestroyTexture(worldMapTexture);

    if (ceilingSurface)
        SDL_FreeSurface(ceilingSurface);
    if (ceilingTexture)
        SDL_DestroyTexture(ceilingTexture);

    if (floorSurface)
        SDL_FreeSurface(floorSurface);
    if (floorTexture)
        SDL_DestroyTexture(floorTexture);

    if (wallSurface)
        SDL_FreeSurface(wallSurface);
    if (wallTexture)
        SDL_DestroyTexture(wallTexture);

    SDL_SetRelativeMouseMode(SDL_FALSE);
    SDL_Quit();

    raycaster.reset();
}

void Render::draw() {
    drawPlayerVision();
    if (showMiniMap)
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
