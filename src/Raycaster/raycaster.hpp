#pragma once

#include "../Common/util.hpp"

class Raycaster {
  private:
    float step;
    const float fov = PI / 3.0f;

    void intersectionRayWithWall(Position &ray, float &distance,
                                 vector<vector<int>> tile, float vAngle,
                                 int count);

  public:
    const int rayCount = 30;
    const float maxDistance = 400.0f;
    struct Ray {
        Position hitPosition;
        float distance;
    };

  public:
    vector<Ray> rayCastWorld(Position start, vector<vector<int>> tile,
                             float vAngle);
    Position rayCastMinimap(Position start, vector<vector<int>> tile,
                            float vAngle, int count);

    Raycaster();
};
