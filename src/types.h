#ifndef TYPES_H
#define TYPES_H

#include <cmath>
#include <concepts>
#include <type_traits>

template <typename T>
concept NumberConcept = std::integral<T> or std::floating_point<T>;

/// A 2D vector.
struct Vector2 {
  float x, y;

  void operator+=(const Vector2& other) {
    x += other.x;
    y += other.y;
  }

  template <NumberConcept T>
  Vector2 operator*(const T num) const {
    return {static_cast<float>(x * num), static_cast<float>(y * num)};
  }

  void normalize() {
    if (x == 0 | y == 0)
      return;
    auto distance = std::sqrt((std::pow(x, 2) + std::pow(y, 2)));
    x = x / distance;
    y = y / distance;
  };
};

#endif
