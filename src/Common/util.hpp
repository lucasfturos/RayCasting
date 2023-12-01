#pragma once

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
    Ground = 0, // Representa a cor do chão
};

// Usings
using Position = Point<float>;
using Velocity = Point<float>;
using Plane = Point<float>;

using Direction = Point<int>;
using Mouse = Point<int>;

using std::make_shared;
using std::shared_ptr;
using std::vector;
