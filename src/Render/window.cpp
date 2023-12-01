#include "render.hpp"

void Render::initWindow() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw std::runtime_error(std::string("SDL_Init Error: ") +
                                 SDL_GetError());
    }

    window = SDL_CreateWindow("Raycasting", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT,
                              SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
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
        return (*p > threshold) ? ColorTilemap::Wall : ColorTilemap::Ground;
    case 2:
        return (*reinterpret_cast<Uint16 *>(p) > threshold)
                   ? ColorTilemap::Wall
                   : ColorTilemap::Ground;
    case 3:
        return ((static_cast<Uint32>(p[0]) + p[1] + p[2]) / 3 > threshold)
                   ? ColorTilemap::Wall
                   : ColorTilemap::Ground;
    case 4:
        return (*reinterpret_cast<Uint32 *>(p) > threshold)
                   ? ColorTilemap::Wall
                   : ColorTilemap::Ground;
    default:
        return 0;
    }
}

void Render::loadTilemap() {
    worldMapSurface = SDL_LoadBMP("./img/Mapa.bmp");
    if (!worldMapSurface) {
        throw std::runtime_error(std::string("SDL_LoadBMP Error: ") +
                                 SDL_GetError());
    }

    worldMapTexture = SDL_CreateTextureFromSurface(renderer, worldMapSurface);
    if (!worldMapTexture) {
        SDL_FreeSurface(worldMapSurface);
        throw std::runtime_error(
            std::string("SDL_CreateTextureFromSurface Error: ") +
            SDL_GetError());
    }

    SDL_QueryTexture(worldMapTexture, nullptr, nullptr, &wordlMapWidth,
                     &wordlMapHeight);

    map = Map(wordlMapWidth, wordlMapHeight);
    for (int y = 0; y < wordlMapHeight; ++y) {
        for (int x = 0; x < wordlMapWidth; ++x) {
            Uint32 pixel = getPixel(worldMapSurface, x, y, 0x80);
            map.tile[y][x] = pixel;
        }
    }
}
