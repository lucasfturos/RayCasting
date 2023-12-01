#include "raycaster.hpp"

Raycaster::Raycaster() { step = fov / static_cast<float>(rayCount); }

void Raycaster::intersectionRayWithWall(Position &ray, float &distance,
                                        vector<vector<int>> tile, float vAngle,
                                        int count) {
    float angle = vAngle + (count - rayCount / 2.0f) * step;
    // Encontrar interseção com a parede
    while (tile[static_cast<int>(ray.y)][static_cast<int>(ray.x)] == 0 &&
           distance < maxDistance) {
        ray.x += cos(angle);
        ray.y += sin(angle);
        distance += 1.0f;
    }
}

Position Raycaster::rayCastMinimap(Position start, vector<vector<int>> tile,
                                   float vAngle, int count) {
    Position ray = start;
    float distance = 0.0f;
    intersectionRayWithWall(ray, distance, tile, vAngle, count);
    return ray;
}

std::vector<Raycaster::RayResult>
Raycaster::rayCastWorld(Position start, vector<vector<int>> tile,
                        float vAngle) {
    std::vector<RayResult> results;
    for (auto count{0}; count < rayCount; ++count) {
        Position ray = start;
        float distance = 0.0f;
        intersectionRayWithWall(ray, distance, tile, vAngle, count);
        results.push_back({ray, distance});
    }
    return results;
}
