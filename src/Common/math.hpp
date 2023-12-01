#pragma once

#include <cmath>

template <typename T> inline T length(T x, T y) { return sqrt(x * x + y * y); }

template <typename T> inline T normalize(T &x, T &y) {
    T len = length(x, y);
    if (len != 0) {
        x /= len;
        y /= len;
    }
    return len;
}

template <typename T> inline T clamp(T value, T min, T max) {
    return (value < min) ? min : (value > max) ? max : value;
}

const float PI = M_PI;
