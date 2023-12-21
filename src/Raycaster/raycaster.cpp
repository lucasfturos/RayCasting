#include "raycaster.hpp"

Raycaster::Raycaster() { step = fov / static_cast<float>(rayCount); }

void Raycaster::intersectionRayWithWall(Position &ray, float &distance,
                                        vector<vector<int>> tile, float vAngle,
                                        int count) {
    float angle = vAngle + (count - rayCount / 2.0f) * step;

    float xStep = cos(angle);
    float yStep = sin(angle);

    float x = ray.x;
    float y = ray.y;

    float deltaDistX = std::abs(1.0f / xStep);
    float deltaDistY = std::abs(1.0f / yStep);

    int stepX, stepY;
    float sideDistX, sideDistY;

    if (xStep < 0) {
        stepX = -1;
        sideDistX = (x - static_cast<int>(x)) * deltaDistX;
    } else {
        stepX = 1;
        sideDistX = (static_cast<int>(x) + 1.0 - x) * deltaDistX;
    }

    if (yStep < 0) {
        stepY = -1;
        sideDistY = (y - static_cast<int>(y)) * deltaDistY;
    } else {
        stepY = 1;
        sideDistY = (static_cast<int>(y) + 1.0 - y) * deltaDistY;
    }

    int hit = 0;
    while (hit == 0 && distance < maxDistance) {
        if (sideDistX < sideDistY) {
            sideDistX += deltaDistX;
            x += stepX;
            side = 0;
        } else {
            sideDistY += deltaDistY;
            y += stepY;
            side = 1;
        }
        if (tile[static_cast<int>(y)][static_cast<int>(x)] > 0)
            hit = 1;

        distance += 1.0f;
    }

    ray.x = x;
    ray.y = y;
}

Position Raycaster::rayCastMinimap(Position start, vector<vector<int>> tile,
                                   float vAngle, int count) {
    Position ray = start;
    float distance = 0.0f;
    intersectionRayWithWall(ray, distance, tile, vAngle, count);
    return ray;
}

vector<Raycaster::Ray> Raycaster::rayCastWorld(Position start,
                                               vector<vector<int>> tile,
                                               float vAngle) {
    vector<Ray> results;
    for (auto count{0}; count < rayCount; ++count) {
        Position ray = start;
        float distance = 0.0f;
        float direction = vAngle * step;
        intersectionRayWithWall(ray, distance, tile, vAngle, count);
        results.push_back({ray, distance, direction, side});
    }
    return results;
}
