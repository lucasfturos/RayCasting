#include "render.hpp"

void Render::updateMovements() {
    updatePlayerPosition();
    updateCameraPosition();
}

void Render::updatePlayerPosition() {
    Position next = playerPos;

    float moveSpeed = speed * deltaTime;

    if (playerVelocity.y != 0) {
        next.x += playerVelocity.y * std::cos(cameraAngle) * moveSpeed;
        next.y += playerVelocity.y * std::sin(cameraAngle) * moveSpeed;
    }

    if (playerVelocity.x != 0) {
        next.x += playerVelocity.x * std::cos(cameraAngle + M_PI_2) * moveSpeed;
        next.y += playerVelocity.x * std::sin(cameraAngle + M_PI_2) * moveSpeed;
    }

    if (!checkPlayerCollision(next)) {
        playerPos = next;
    }
}

void Render::updateCameraPosition() {
    float radius = 720.0f;
    cameraPos.x = playerPos.x + std::cos(cameraAngle) * radius;
    cameraPos.y = playerPos.y + std::sin(cameraAngle) * radius;
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
        std::min(worldMapHeight - 1, tileY + static_cast<int>(radiusObjPlayer));
    int startX = std::max(0, tileX - static_cast<int>(radiusObjPlayer));
    int endX =
        std::min(worldMapWidth - 1, tileX + static_cast<int>(radiusObjPlayer));

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
