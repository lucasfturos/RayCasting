#include "raycaster.hpp"

Raycaster::Raycaster() { step = fov / static_cast<float>(rayCount); }

Position Raycaster::rayCastMinimap(Position start, vector<vector<int>> tile,
                                   float vAngle, int count) {
    float angle = vAngle + (count - rayCount / 2.0f) * step;

    // Defina a distância máxima que um raio pode percorrer
    const float maxDistance = 400.0f;

    // Encontrar interseção com a parede
    Position ray = start;
    float distance = 0.0f; // Distância percorrida pelo raio
    while (tile[static_cast<int>(ray.y)][static_cast<int>(ray.x)] == 0 &&
           distance < maxDistance) {
        ray.x += cos(angle);
        ray.y += sin(angle);
        distance += 1.0f; // Incrementa a distância a cada passo
    }
    return ray;
}

std::vector<Raycaster::RayResult>
Raycaster::rayCastWorld(Position start, const Map &map, float playerAngle) {
    std::vector<RayResult> results;

    for (int count = 0; count < rayCount; ++count) {
        float angle = playerAngle + (count - rayCount / 2.0f) * step;

        // Encontra interseção com a parede
        Position ray = start;
        float distance = 0.0f; // Distância percorrida pelo raio
        while (map.tile[static_cast<int>(ray.y)][static_cast<int>(ray.x)] ==
                   0 &&
               distance < maxDistance) {
            ray.x += cos(angle);
            ray.y += sin(angle);
            distance += 1.0f; // Incrementa a distância a cada passo
        }

        results.push_back({ray, distance});
    }

    return results;
}
