#include "raycaster.hpp"

Raycaster::Raycaster() : step(fov / static_cast<float>(rayCount)) {}

void Raycaster::calculateStepAndSide(const float &angle, const float &x,
                                     const float &y, int &stepX, int &stepY,
                                     float &sideDistX, float &sideDistY,
                                     float &deltaDistX,
                                     float &deltaDistY) const {
    float xStep = std::cos(angle);
    float yStep = std::sin(angle);

    deltaDistX = std::abs(1.0f / xStep);
    deltaDistY = std::abs(1.0f / yStep);

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
}

void Raycaster::intersectionRayWithWall(Position &ray, float &distance,
                                        const vector<vector<int>> &tile,
                                        float angle, int &side) const {
    float x = ray.x;
    float y = ray.y;
    int stepX, stepY;
    float sideDistX, sideDistY, deltaDistX, deltaDistY;
    calculateStepAndSide(angle, x, y, stepX, stepY, sideDistX, sideDistY,
                         deltaDistX, deltaDistY);

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

Position Raycaster::rayCastMinimap(const Position &start,
                                   const vector<vector<int>> &tile,
                                   float vAngle, int count) const {
    Position ray = start;
    float distance = 0.0f;
    int side = 0;
    float angle = vAngle + (count - rayCount / 2.0f) * step;
    intersectionRayWithWall(ray, distance, tile, angle, side);
    return ray;
}

vector<Raycaster::Ray> Raycaster::rayCastWorld(const Position &start,
                                               const vector<vector<int>> &tile,
                                               float vAngle) const {
    vector<Ray> results;
    for (int count = 0; count < rayCount; ++count) {
        Position ray = start;
        float distance = 0.0f;
        int side = 0;
        float angle = vAngle + (count - rayCount / 2.0f) * step;
        intersectionRayWithWall(ray, distance, tile, angle, side);
        results.push_back({ray, distance, angle, side});
    }
    return results;
}
