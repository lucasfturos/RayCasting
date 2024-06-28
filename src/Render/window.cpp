#include "render.hpp"

void Render::initWindow() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw std::runtime_error(std::string("SDL_Init Error: ") +
                                 SDL_GetError());
    }

    window = SDL_CreateWindow("Raycasting", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT,
                              SDL_WINDOW_VULKAN);
    if (window == nullptr) {
        throw std::runtime_error(std::string("SDL_CreateWindow Error: ") +
                                 SDL_GetError());
    }

    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_SOFTWARE | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") +
                                 SDL_GetError());
    }
}

Uint32 Render::getPixel(SDL_Surface *surface, int x, int y, Uint32 threshold) {
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p =
        static_cast<Uint8 *>(surface->pixels) + y * surface->pitch + x * bpp;

    switch (bpp) {
    case 1:
        return (*p > threshold) ? ColorTilemap::Wall : ColorTilemap::Floor;
    case 2:
        return (*reinterpret_cast<Uint16 *>(p) > threshold)
                   ? ColorTilemap::Wall
                   : ColorTilemap::Floor;
    case 3:
        return ((static_cast<Uint32>(p[0]) + p[1] + p[2]) / 3 > threshold)
                   ? ColorTilemap::Wall
                   : ColorTilemap::Floor;
    case 4:
        return (*reinterpret_cast<Uint32 *>(p) > threshold)
                   ? ColorTilemap::Wall
                   : ColorTilemap::Floor;
    default:
        return 0;
    }
}

void Render::initTexture() {
    loadTilemap();
    loadCeiling();
    loadWall();
    loadFloor();
}

void Render::loadWall() {
    wallSurface = SDL_LoadBMP("./img/Wall/wall.bmp");
    !wallSurface ? throw std::runtime_error(std::string("SDL_LoadBMP Error: ") +
                                            SDL_GetError())
                 : 0;
    wallTexture = SDL_CreateTextureFromSurface(renderer, wallSurface);
    if (!wallTexture) {
        SDL_FreeSurface(wallSurface);
        throw std::runtime_error(
            std::string("SDL_CreateTextureFromSurface Error: ") +
            SDL_GetError());
    }
}

void Render::loadFloor() {
    floorSurface = SDL_LoadBMP("./img/Floor/Stontex.bmp");
    !floorSurface ? throw std::runtime_error(
                        std::string("SDL_LoadBMP Error: ") + SDL_GetError())
                  : 0;

    floorTexture = SDL_CreateTextureFromSurface(renderer, floorSurface);
    if (!floorTexture) {
        SDL_FreeSurface(floorSurface);
        throw std::runtime_error(
            std::string("SDL_CreateTextureFromSurface Error: ") +
            SDL_GetError());
    }
}

void Render::loadCeiling() {
    ceilingSurface = SDL_LoadBMP("./img/Ceiling/Stone.bmp");
    !ceilingSurface
        ? throw std::runtime_error(std::string("IMG_LoadTexture Error: ") +
                                   SDL_GetError())
        : 0;

    ceilingTexture = SDL_CreateTextureFromSurface(renderer, ceilingSurface);
    if (!ceilingTexture) {
        SDL_FreeSurface(ceilingSurface);
        throw std::runtime_error(
            std::string("SDL_CreateTextureFromSurface Error: ") +
            SDL_GetError());
    }
}

void Render::loadTilemap() {
    worldMapSurface = SDL_LoadBMP("./img/Mapa/Mapa.bmp");
    !worldMapSurface ? throw std::runtime_error(
                           std::string("SDL_LoadBMP Error: ") + SDL_GetError())
                     : 0;

    worldMapTexture = SDL_CreateTextureFromSurface(renderer, worldMapSurface);
    if (!worldMapTexture) {
        SDL_FreeSurface(worldMapSurface);
        throw std::runtime_error(
            std::string("SDL_CreateTextureFromSurface Error: ") +
            SDL_GetError());
    }

    SDL_QueryTexture(worldMapTexture, nullptr, nullptr, &worldMapWidth,
                     &worldMapHeight);

    map = Map(worldMapWidth, worldMapHeight);
    for (int y = 0; y < worldMapHeight; ++y) {
        for (int x = 0; x < worldMapWidth; ++x) {
            Uint32 pixel = getPixel(worldMapSurface, x, y, 0x80);
            map.tile[y][x] = pixel;
        }
    }
}
