#pragma once

#include "../Common/util.hpp"

class Raycaster {
  private:
    int side;
    float step;
    const float fov = PI / 3.0f;

    void intersectionRayWithWall(Position &ray, float &distance,
                                 vector<vector<int>> tile, float vAngle,
                                 int count);

  public:
    const int rayCount = 40;
    const float maxDistance = 300.0f;
    struct Ray {
        Position hitPosition;
        float distance;
        float direction;
        int side;
    };

  public:
    vector<Ray> rayCastWorld(Position start, vector<vector<int>> tile,
                             float vAngle);
    Position rayCastMinimap(Position start, vector<vector<int>> tile,
                            float vAngle, int count);

    Raycaster();
};
