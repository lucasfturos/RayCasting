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
    // Ajusta a posição e largura da coluna
    int x1 = column * (WIDTH / raycaster->rayCount);
    int x2 = x1 + WIDTH / raycaster->rayCount;
    int y2 = (HEIGHT + wallHeight) / 2;

    // Desenha o chão
    SDL_Rect floorRect = {
        .x = x1,
        .y = y2,
        .w = x2 - x1,
        .h = HEIGHT - y2,
    };
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_RenderFillRect(renderer, &floorRect);
}

void Render::drawCeiling(int column, float wallHeight) {
    // Ajusta a posição e largura da coluna
    int x1 = column * (WIDTH / raycaster->rayCount);
    int x2 = x1 + WIDTH / raycaster->rayCount;
    int y1 = (HEIGHT - wallHeight) / 2;

    // Desenha o teto
    SDL_Rect ceilingRect = {
        .x = x1,
        .y = 0,
        .w = x2 - x1,
        .h = y1,
    };
    SDL_SetRenderDrawColor(renderer, 0, 0, 128, 255); // Cor azul para o teto
    SDL_RenderFillRect(renderer, &ceilingRect);
}

void Render::drawWall(int column, float wallHeight) {
    int x = column * (WIDTH / raycaster->rayCount);
    int width = WIDTH / raycaster->rayCount;
    int y = (HEIGHT - wallHeight) / 2;

    SDL_Rect rect = {
        .x = x,
        .y = y,
        .w = width,
        .h = static_cast<int>(wallHeight),
    };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void Render::drawRaycaster(const std::vector<Raycaster::Ray> &rays) {
    for (auto i{0}; i < raycaster->rayCount; ++i) {
        const Raycaster::Ray &ray = rays[i];

        float normalizedDistance =
            1.0f - (ray.distance / raycaster->maxDistance);
        normalizedDistance = std::max(0.0f, std::min(1.0f, normalizedDistance));

        int sizeWall = 200;
        float wallHeight = normalizedDistance * sizeWall;

        drawCeiling(i, wallHeight);
        drawWall(i, wallHeight);
        drawFloor(i, wallHeight);
    }
}

void Render::drawPlayerVision() {
    float vAngle = atan2(mousePos.y - cameraPos.y, mousePos.x - cameraPos.x);
    Position start = playerPos;
    vector<Raycaster::Ray> rays =
        raycaster->rayCastWorld(start, map.tile, vAngle);
    drawRaycaster(rays);
}
