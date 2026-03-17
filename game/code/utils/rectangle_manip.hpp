#pragma once
#include "pch.hpp"

static Rectangle RectangleZero() {
  return { 0, 0, 0, 0 };
}

static bool RectangleEquals(cref<Rectangle> a, cref<Rectangle> b) {
  return
    a.x == b.x and a.y == b.y
    and
    a.width == b.width and a.height == b.height;
}

static Rectangle RectangleSize(cref<Vector2> size) {
  return { 0, 0, size.x, size.y };
}