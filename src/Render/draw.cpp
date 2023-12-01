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

void Render::drawVerticalColumn(int column, float wallHeight) {
    // Ajusta a posição e largura da coluna
    int x1 = column * (WIDTH / raycaster->rayCount);
    int x2 = x1 + WIDTH / raycaster->rayCount;
    int y1 = (HEIGHT - wallHeight) / 2;
    int y2 = y1 + wallHeight;

    // Desenha apenas a borda do retângulo
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Linha à esquerda
    SDL_RenderDrawLine(renderer, x1, y1, x1, y2);
    // Linha superior
    SDL_RenderDrawLine(renderer, x1, y1, x2, y1);
    // Linha à direita
    SDL_RenderDrawLine(renderer, x2, y1, x2, y2);
    // Linha inferior
    SDL_RenderDrawLine(renderer, x1, y2, x2, y2);

    /*
    int x = column * (WIDTH / raycaster->rayCount);
    int width = WIDTH / raycaster->rayCount;
    int y = (HEIGHT - wallHeight) / 2;

    // Desenha a coluna vertical
    SDL_Rect rect = {
        .x = x,
        .y = y,
        .w = width,
        .h = static_cast<int>(wallHeight),
    };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
    */
}

void Render::drawRaycasterResults(
    const std::vector<Raycaster::RayResult> &results) {
    for (auto i{0}; i < raycaster->rayCount; ++i) {
        const Raycaster::RayResult &result = results[i];

        // Mapeia a distância para o intervalo [0, 1]
        float normalizedDistance =
            1.0f - (result.distance / raycaster->maxDistance);

        // Limita o valor no intervalo [0, 1]
        normalizedDistance = std::max(0.0f, std::min(1.0f, normalizedDistance));

        // Mapeia a altura para o intervalo desejado
        float wallHeight = normalizedDistance * 200;

        // Desenha a coluna vertical correspondente
        drawVerticalColumn(i, wallHeight);
    }
}

void Render::drawPlayerVision() {
    float vAngle = atan2(mousePos.y - cameraPos.y, mousePos.x - cameraPos.x);
    // Posição inicial dos raios
    Position start = playerPos;

    std::vector<Raycaster::RayResult> results =
        raycaster->rayCastWorld(start, map.tile, vAngle);

    // Desenha as colunas verticais com base nos resultados dos raios
    drawRaycasterResults(results);
}
