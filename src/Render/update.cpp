#include "render.hpp"

void Render::updateMovements() {
    updatePlayerPosition();
    updateCameraPosition();
}

void Render::updatePlayerPosition() {
    Position next = {0, 0};
    next.x += playerPos.x + playerVelocity.x * deltaTime;
    next.y += playerPos.y + playerVelocity.y * deltaTime;

    if (!checkPlayerCollision(next)) {
        playerPos = {next.x, next.y};
    }
}

void Render::updateCameraPosition() {
    float radius = 50.0f;
    float angle = atan2(mousePos.y - playerPos.y, mousePos.x - playerPos.x);
    cameraPos.x = playerPos.x + cos(angle) * radius;
    cameraPos.y = playerPos.y + sin(angle) * radius;
}

bool Render::checkPlayerCollision(Position next) {
    int tileX = static_cast<int>(next.x);
    int tileY = static_cast<int>(next.y);

    float left = next.x - radiusObjPlayer;
    float right = next.x + radiusObjPlayer;
    float top = next.y - radiusObjPlayer;
    float bottom = next.y + radiusObjPlayer;

    int startY = std::max(0, tileY - static_cast<int>(radiusObjPlayer));
    int endY =
        std::min(wordlMapHeight - 1, tileY + static_cast<int>(radiusObjPlayer));
    int startX = std::max(0, tileX - static_cast<int>(radiusObjPlayer));
    int endX =
        std::min(wordlMapWidth - 1, tileX + static_cast<int>(radiusObjPlayer));

    for (int y = startY; y <= endY; ++y) {
        for (int x = startX; x <= endX; ++x) {
            if (map.tile[y][x] == ColorTilemap::Wall) {
                if (x >= left && x <= right && y >= top && y <= bottom) {
                    return true;
                }
            }
        }
    }

    return false;
}
