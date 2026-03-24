#pragma once
#include "pch.hpp"

namespace vs {
  inline Rectangle RectangleZero() {
    return { 0, 0, 0, 0 };
  }

  inline Vector2 RectangleGetPos(cref<Rectangle> rectangle) {
    return { rectangle.x, rectangle.y };
  }
  inline Vector2 RectangleGetSize(cref<Rectangle> rectangle) {
    return { rectangle.width, rectangle.height };
  }

  inline bool RectangleEquals(cref<Rectangle> a, cref<Rectangle> b) {
    return
      a.x == b.x and a.y == b.y
      and
      a.width == b.width and a.height == b.height;
  }

  inline Rectangle RectangleFromVectors(cref<Vector2> pos, cref<Vector2> size) {
    return { pos.x, pos.y, size.x, size.y };
  }
  inline Rectangle RectangleFromSize(cref<Vector2> size) {
    return { 0, 0, size.x, size.y };
  }

  inline Rectangle RectangleAddRec(Rectangle rec, cref<Rectangle> rec2) {
    rec.x += rec2.x;
    rec.y += rec2.y;
    rec.width += rec2.width;
    rec.height += rec2.height;
    return rec;
  }
  inline Rectangle RectangleAddPos(Rectangle rec, cref<Vector2> pos) {
    rec.x += pos.x;
    rec.y += pos.y;
    return rec;
  }
  inline Rectangle RectangleAddSize(Rectangle rec, cref<Vector2> size) {
    rec.width += size.x;
    rec.height += size.y;
    return rec;
  }

  inline Rectangle RectangleGetWorldToScreen(Rectangle rectangle, cref<Camera2D> camera) {
    const auto [x, y] = GetWorldToScreen2D({rectangle.x, rectangle.y}, camera);

    rectangle.x = x;
    rectangle.y = y;
    rectangle.width *= camera.zoom;
    rectangle.height *= camera.zoom;

    return rectangle;
  }

  inline void DrawRectangleFromSize(cref<Vector2> size, cref<Color> color) {
    DrawRectangleRec(RectangleFromSize(size), color);
  }
  inline void DrawRectangleFromVectors(cref<Vector2> pos, cref<Vector2> size, cref<Color> color) {
    DrawRectangleRec(RectangleFromVectors(pos, size), color);
  }
}