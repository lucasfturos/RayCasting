#include "raycaster.hpp"

Raycaster::Raycaster() { step = fov / static_cast<float>(rayCount); }

void Raycaster::intersectionRayWithWall(Position &ray, float &distance,
                                        vector<vector<int>> tile, float vAngle,
                                        int count) {
    float angle = vAngle + (count - rayCount / 2.0f) * step;

    // Adiciona um pequeno deslocamento à posição inicial do raio
    ray.x += cos(angle) * 0.01f;
    ray.y += sin(angle) * 0.01f;

    float xStep = cos(angle);
    float yStep = sin(angle);

    // Inicializa variáveis de DDA
    float x = ray.x;
    float y = ray.y;

    // Encontrar interseção com a parede usando o algoritmo DDA
    while (tile[static_cast<int>(y)][static_cast<int>(x)] == 0 &&
           distance < maxDistance) {
        x += xStep;
        y += yStep;
        distance += 1.0f;
    }

    // Atualiza a posição final do raio
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
        intersectionRayWithWall(ray, distance, tile, vAngle, count);
        results.push_back({ray, distance});
    }
    return results;
}
