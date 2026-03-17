#pragma once
#include "pch.hpp"

namespace vs::frame {
  inline bool window_resized;
  inline bool window_focused;

  inline Vector2 mouse_position;
  inline Vector2 mouse_delta;

  inline void dispatch() {
    window_resized = IsWindowResized();
    window_focused = IsWindowFocused();

    mouse_position = GetMousePosition();
    mouse_delta = GetMouseDelta();
  }
}
