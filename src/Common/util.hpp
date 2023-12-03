#pragma once

#include <cmath>
#include <memory>
#include <vector>

// Structs
template <typename T> struct Point {
    T x, y;
};

struct Map {
    std::vector<std::vector<int>> tile;
    Map() {}
    Map(int width, int height) : tile(height, std::vector<int>(width, 0)) {}
};

// Enums
enum ColorTilemap : int {
    Wall = 1,   // Representa a cor das paredes
    Floor = 0, // Representa a cor do chão
};

// Usings
using Position = Point<float>;
using Velocity = Point<float>;

using Direction = Point<int>;
using Mouse = Point<int>;

using std::make_shared;
using std::shared_ptr;
using std::vector;

// Constants
const float PI = M_PI;
