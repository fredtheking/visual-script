#pragma once
#include "pch.hpp"
#include "types/vector2.hpp"

namespace vs::options {
  static Vector2i window_size;
  static double window_size_aspect_ratio;

  static string window_title = "mc clone";

  static int player_fov = 100;

  static void set_window_size(cref<Vector2i> new_size) {
    window_size = new_size;
    window_size_aspect_ratio =
      static_cast<double>(window_size.x)
      /
      static_cast<double>(window_size.y);
  }
  static void set_window_title(const string_view new_title) {
    window_title = new_title.data();
  }
}
