#pragma once

#include "../Common/util.hpp"

using std::vector;

class Raycaster {
  public:
    const int rayCount = 30;
    const float maxDistance = 200.0f;
    struct Ray {
        Position hitPosition;
        float distance;
        float direction;
        int side;
    };

  private:
    const float fov = PI / 3.0f;
    float step;

    void intersectionRayWithWall(Position &ray, float &distance,
                                 const vector<vector<int>> &tile, float angle,
                                 int &side) const;

  public:
    Raycaster();

    vector<Ray> rayCastWorld(const Position &start,
                             const vector<vector<int>> &tile,
                             float vAngle) const;
    Position rayCastMinimap(const Position &start,
                            const vector<vector<int>> &tile, float vAngle,
                            int count) const;
};
