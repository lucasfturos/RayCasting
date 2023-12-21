#include "render.hpp"

void Render::drawCircle(SDL_Renderer *renderer, float x, float y, int radius,
                        SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    for (float angle = 0; angle < 360; angle += 0.1) {
        float radianAngle = deg_to_rad(angle);
        int drawX = static_cast<int>(x + radius * cos(radianAngle));
        int drawY = static_cast<int>(y + radius * sin(radianAngle));

        SDL_RenderDrawPoint(renderer, drawX, drawY);
    }
}

void Render::drawMiniMap() {
    int miniMapWidth = 200;
    int miniMapHeight = 150;
    float scaleX = miniMapWidth / static_cast<float>(wordlMapWidth);
    float scaleY = miniMapHeight / static_cast<float>(wordlMapHeight);
    int miniMapX = 10;
    int miniMapY = 10;

    drawTilemapOnMiniMap(miniMapX, miniMapY, miniMapWidth, miniMapHeight);
    drawPlayerOnMiniMap(miniMapX, miniMapY, scaleX, scaleY);
    drawPlayerVisionOnMiniMap(miniMapX, miniMapY, scaleX, scaleY);
}

void Render::drawTilemapOnMiniMap(int x, int y, int width, int height) {
    SDL_Rect dstRect = {.x = x, .y = y, .w = width, .h = height};
    SDL_Rect srcRect = {
        .x = 0, .y = 0, .w = wordlMapWidth, .h = wordlMapHeight};
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
        .y = playerPos.y + (radiusObjPlayer - margin) * sin(vAngle),
    };

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

    distanceToCenter = std::hypot((x1 + x2) / 2 - WIDTH / 2, y2 - HEIGHT / 2);
    maskIntensity = 1.0f - (distanceToCenter / maskRadius);
    brightnessFactor *= std::max(0.0f, maskIntensity);

    SDL_Rect floorRect = {
        .x = x1,
        .y = y2,
        .w = x2 - x1,
        .h = HEIGHT - y2,
    };

    // SDL_RenderCopy(renderer, floorTexture, nullptr, &floorRect);
    SDL_SetRenderDrawColor(renderer, brightnessFactor * 150,
                           brightnessFactor * 150, brightnessFactor * 150,
                           brightnessFactor * 255);
    SDL_RenderFillRect(renderer, &floorRect);
}

void Render::drawCeiling(int column, float wallHeight) {
    int x1 = column * (WIDTH / raycaster->rayCount);
    int x2 = (column + 1) * WIDTH / raycaster->rayCount;
    int y1 = (HEIGHT - wallHeight) / 2;

    distanceToCenter = std::hypot((x1 + x2) / 2 - WIDTH / 2, y1 - HEIGHT / 2);
    maskIntensity = 1.0f - (distanceToCenter / maskRadius);
    brightnessFactor *= std::max(0.0f, maskIntensity);

    SDL_Rect ceilingRect = {
        .x = x1,
        .y = 0,
        .w = x2 - x1,
        .h = y1,
    };

    // SDL_RenderCopy(renderer, ceilingTexture, nullptr, &ceilingRect);
    SDL_SetRenderDrawColor(renderer, brightnessFactor * 60,
                           brightnessFactor * 60, brightnessFactor * 60,
                           brightnessFactor * 255);
    SDL_RenderFillRect(renderer, &ceilingRect);
}

void Render::drawWall(int column, float wallHeight, int side,
                      float distanceToWall) {
    int x = column * (WIDTH / raycaster->rayCount);
    int width = WIDTH / raycaster->rayCount;
    int y = (HEIGHT - wallHeight) / 2;

    int textureHeight = static_cast<int>(HEIGHT / distanceToWall);
    int textureWidth = wallSurface->w;

    int textureY =
        static_cast<int>((wallHeight / 2.0f) * textureHeight / wallHeight);

    int numRepetitions = 10;
    SDL_Rect textureRect = {
        .x = 0,
        .y = textureY,
        .w = textureWidth,
        .h = textureHeight,
    };
    SDL_Rect wallRect = {
        .x = x,
        .y = y,
        .w = width * numRepetitions,
        .h = static_cast<int>(wallHeight),
    };

    brightnessFactor = 1.0f - (distanceToWall / raycaster->maxDistance);
    brightnessFactor = std::max(0.2f, brightnessFactor);
    brightnessFactor *= (side == 1) ? 0.5f : 1.0f;

    SDL_SetTextureColorMod(wallTexture, brightnessFactor * 255,
                           brightnessFactor * 255, brightnessFactor * 255);

    SDL_RenderCopy(renderer, wallTexture, &textureRect, &wallRect);
}

void Render::drawRaycaster(const vector<Raycaster::Ray> &rays) {
    for (auto i{0}; i < raycaster->rayCount; ++i) {
        const Raycaster::Ray &ray = rays[i];

        float correctedDistance = ray.distance * cos(deg_to_rad(ray.direction));
        float normalizedDistance =
            1.0f - (correctedDistance / raycaster->maxDistance);
        normalizedDistance = std::max(0.0f, std::min(1.0f, normalizedDistance));
        int sizeWall = 300;
        float wallHeight = normalizedDistance * sizeWall;

        drawCeiling(i, wallHeight);
        drawWall(i, wallHeight, ray.side, ray.distance);
        drawFloor(i, wallHeight);
    }
}

void Render::drawPlayerVision() {
    float vAngle = atan2(mousePos.y - cameraPos.y, mousePos.x - cameraPos.x);
    Position start = playerPos;
    vector<Raycaster::Ray> rays =
        raycaster->rayCastWorld(start, map.tile, vAngle);

    maskRadius = raycaster->maxDistance;

    for (auto i{0}; i < raycaster->rayCount; ++i) {
        const Raycaster::Ray &ray = rays[i];

        // Ajuste de brilho para a função drawCircle
        brightnessFactor = 1.0f - (ray.distance / raycaster->maxDistance);
        brightnessFactor = std::max(0.2f, brightnessFactor);

        // Ajuste de intensidade da máscara
        distanceToCenter =
            std::hypot((WIDTH / 2.0f) - WIDTH / 2.0f, 0 - HEIGHT / 2.0f);
        maskIntensity = 1.0f - (distanceToCenter / maskRadius);
        brightnessFactor *= std::max(0.0f, maskIntensity);

        // Desenhar o raio apenas se estiver dentro do campo de visão
        if (i >= raycaster->rayCount / 4 && i <= 3 * raycaster->rayCount / 4) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0,
                                   static_cast<Uint8>(255 * brightnessFactor));
            SDL_RenderDrawLineF(
                renderer, WIDTH / 2.0f, HEIGHT / 2.0f,
                WIDTH / 2.0f + ray.distance * cos(ray.direction),
                HEIGHT / 2.0f + ray.distance * sin(ray.direction));
        }
    }

    drawRaycaster(rays);
}
