#include "render.hpp"

void Render::drawCircle(SDL_Renderer *renderer, float x, float y, int radius,
                        SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    int cx = radius;
    int cy = 0;
    int radiusError = 1 - cx;

    while (cx >= cy) {
        SDL_RenderDrawPoint(renderer, x + cx, y - cy);
        SDL_RenderDrawPoint(renderer, x - cx, y - cy);
        SDL_RenderDrawPoint(renderer, x + cx, y + cy);
        SDL_RenderDrawPoint(renderer, x - cx, y + cy);
        SDL_RenderDrawPoint(renderer, x + cy, y - cx);
        SDL_RenderDrawPoint(renderer, x - cy, y - cx);
        SDL_RenderDrawPoint(renderer, x + cy, y + cx);
        SDL_RenderDrawPoint(renderer, x - cy, y + cx);
        cy++;

        if (radiusError < 0) {
            radiusError += 2 * cy + 1;
        } else {
            cx--;
            radiusError += 2 * (cy - cx + 1);
        }
    }
}

void Render::drawMiniMap() {
    int miniMapWidth = 200;
    int miniMapHeight = 150;
    float scaleX = miniMapWidth / static_cast<float>(worldMapWidth);
    float scaleY = miniMapHeight / static_cast<float>(worldMapHeight);
    int miniMapX = 10;
    int miniMapY = 10;

    drawTilemapOnMiniMap(miniMapX, miniMapY, miniMapWidth, miniMapHeight);
    drawPlayerOnMiniMap(miniMapX, miniMapY, scaleX, scaleY);
    drawPlayerVisionOnMiniMap(miniMapX, miniMapY, scaleX, scaleY);
}

void Render::drawTilemapOnMiniMap(int x, int y, int width, int height) {
    SDL_Rect dstRect = {.x = x, .y = y, .w = width, .h = height};
    SDL_Rect srcRect = {
        .x = 0, .y = 0, .w = worldMapWidth, .h = worldMapHeight};
    SDL_RenderCopy(renderer, worldMapTexture, &srcRect, &dstRect);
}

void Render::drawPlayerOnMiniMap(int x, int y, float scaleX, float scaleY) {
    float playerScale = radiusObjPlayer * scaleX;
    drawCircle(renderer, x + playerPos.x * scaleX, y + playerPos.y * scaleY,
               playerScale, objColor);
}

void Render::drawPlayerVisionOnMiniMap(int x, int y, float scaleX,
                                       float scaleY) {
    float vAngle = atan2(mousePos.y - cameraPos.y, mousePos.x - cameraPos.x);
    float margin = 5.0f;
    Position start = {
        .x = playerPos.x + (radiusObjPlayer - margin) * cos(vAngle),
        .y = playerPos.y + (radiusObjPlayer - margin) * sin(vAngle)};

    for (auto i{0}; i < raycaster->rayCount; ++i) {
        Position ray = raycaster->rayCastMinimap(start, map.tile, vAngle, i);
        SDL_SetRenderDrawColor(renderer, objColor.r, objColor.g, objColor.b,
                               objColor.a);
        SDL_RenderDrawLineF(renderer, x + start.x * scaleX,
                            y + start.y * scaleY, x + ray.x * scaleX,
                            y + ray.y * scaleY);
    }
}

void Render::drawFloor(int column, float wallHeight) {
    int x1 = column * (WIDTH / raycaster->rayCount);
    int x2 = (column + 1) * WIDTH / raycaster->rayCount;
    int y2 = (HEIGHT + wallHeight) / 2;

    int tileSize = 1;
    for (int y = y2; y < HEIGHT; y += tileSize) {
        for (int x = x1; x < x2; x += tileSize) {
            SDL_Rect floorRect = {.x = x, .y = y, .w = tileSize, .h = tileSize};
            SDL_Rect textureRect = {
                .x = (x - x1) % tileSize,
                .y = (y - y2) % tileSize,
                .w = tileSize,
                .h = tileSize,
            };
            SDL_RenderCopy(renderer, floorTexture, &textureRect, &floorRect);
        }
    }
}

void Render::drawCeiling(int column, float wallHeight) {
    int x1 = column * (WIDTH / raycaster->rayCount);
    int x2 = (column + 1) * WIDTH / raycaster->rayCount;
    int y2 = (HEIGHT - wallHeight) / 2;

    int tileSize = 1;
    for (int y = 0; y < y2; y += tileSize) {
        for (int x = x1; x < x2; x += tileSize) {
            SDL_Rect ceilingRect = {
                .x = x, .y = y, .w = tileSize, .h = tileSize};
            SDL_Rect textureRect = {
                .x = (x - x1) % tileSize,
                .y = y % tileSize,
                .w = tileSize,
                .h = tileSize,
            };
            SDL_RenderCopy(renderer, ceilingTexture, &textureRect,
                           &ceilingRect);
        }
    }
}

void Render::drawWall(int column, float wallHeight, int side,
                      float distanceToWall) {
    int x = column * (WIDTH / raycaster->rayCount);
    int width = WIDTH / raycaster->rayCount;
    int y = (HEIGHT - wallHeight) / 2;

    int tileSize = 32;
    for (int ty = 0; ty < static_cast<int>(wallHeight); ty += tileSize) {
        SDL_Rect wallRect = {
            .x = x,
            .y = y + ty,
            .w = width,
            .h = std::min(tileSize, static_cast<int>(wallHeight) - ty)};

        SDL_Rect textureRect = {
            .x = 0, .y = ty % tileSize, .w = tileSize, .h = wallRect.h};

        float brightnessFactor = 1.0f - (distanceToWall / 10.0f);
        brightnessFactor = std::max(0.2f, brightnessFactor);
        brightnessFactor *= (side == 1) ? 0.3f : 1.0f;

        SDL_SetTextureColorMod(wallTexture, 255 * brightnessFactor,
                               255 * brightnessFactor, 255 * brightnessFactor);
        SDL_RenderCopy(renderer, wallTexture, &textureRect, &wallRect);
    }
}

void Render::drawRaycaster(const vector<Raycaster::Ray> &rays) {
    for (auto i{0}; i < raycaster->rayCount; ++i) {
        const Raycaster::Ray &ray = rays[i];
        const float correctedDistance =
            ray.distance * std::cos(degreeToRadians(ray.direction));
        const float normalizedDistance =
            std::max(0.0f, std::min(1.0f, 1.0f - (correctedDistance /
                                                  raycaster->maxDistance)));
        const int sizeWall = 300;
        const float wallHeight = normalizedDistance * sizeWall;

        drawFloor(i, wallHeight);
        drawWall(i, wallHeight, ray.side, ray.distance);
        drawCeiling(i, wallHeight);
    }
}

void Render::drawPlayerVision() {
    const float vAngle =
        std::atan2(mousePos.y - cameraPos.y, mousePos.x - cameraPos.x);
    Position start = playerPos;
    auto rays = raycaster->rayCastWorld(start, map.tile, vAngle);

    drawRaycaster(rays);
}
