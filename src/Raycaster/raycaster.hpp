#pragma once

#include "../Common/math.hpp"
#include "../Common/util.hpp"

class Raycaster {
  private:
    const float fov = PI / 3.0f;
    float step;

  public:
    const int rayCount = 30;
    const float maxDistance = 400.0f;
    struct RayResult {
        Position hitPosition;
        float distance;
    };
    std::vector<RayResult> rayCastWorld(Position start, const Map &map,
                                        float playerAngle);

    Position rayCastMinimap(Position start, vector<vector<int>> tile,
                            float vAngle, int count);

    Raycaster();
};
