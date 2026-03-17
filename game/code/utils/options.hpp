#pragma once
#include "pch.hpp"

namespace vs::options {
  inline string window_title;
  inline Vector2 window_size;

  inline void set_window_title(const string_view new_title) {
    window_title = new_title.data();
  }
  inline void set_window_size(cref<Vector2> size) {
    window_size = size;
  }
  inline void set_window_size(const float x, const float y) {
    set_window_size(Vector2{ x, y });
  }
  inline void set_window_size(const float xy) {
    set_window_size(xy, xy);
  }

  inline void _setup() {
    set_window_title("Visual-Script");
    set_window_size(1380, 1020);
  }

  inline void dispatch() {
    if (IsWindowResized())
      set_window_size({
        static_cast<float>(GetScreenWidth()),
        static_cast<float>(GetScreenHeight())
      });
  }
}
