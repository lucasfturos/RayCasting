#include "render.hpp"

void Render::updateMovements() {
    updatePlayerPosition();
    updateCameraPosition();
}

void Render::updatePlayerPosition() {
    Position next = playerPos;

    float moveSpeed = speed * deltaTime;

    if (playerVelocity.y != 0) {
        next.x += playerVelocity.y * dirX * moveSpeed;
        next.y += playerVelocity.y * dirY * moveSpeed;
    }

    if (playerVelocity.x != 0) {
        next.x += playerVelocity.x * planeX * moveSpeed;
        next.y += playerVelocity.x * planeY * moveSpeed;
    }

    if (!checkPlayerCollision(next)) {
        playerPos = next;
    }
}

void Render::updateCameraPosition() {
    float radius = 720.0f;
    cameraPos.x = playerPos.x + dirX * radius;
    cameraPos.y = playerPos.y + dirY * radius;

    planeX = -dirY * 0.66f;
    planeY = dirX * 0.66f;
}

bool Render::checkPlayerCollision(Position next) {
    int tileX = static_cast<int>(next.x);
    int tileY = static_cast<int>(next.y);

    if (map.tile[tileY][tileX] == ColorTilemap::Wall) {
        float distX = next.x - (tileX + 0.5f);
        float distY = next.y - (tileY + 0.5f);
        float distance = std::sqrt(distX * distX + distY * distY);

        if (distance < radiusObjPlayer) {
            return true;
        }
    }

    return false;
}
