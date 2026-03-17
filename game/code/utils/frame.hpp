#pragma once
#include "pch.hpp"

namespace vs::frame {
  inline Vector2 window_size;
  inline bool window_resized;
  inline bool window_focused;

  inline Vector2 mouse_position;
  inline Vector2 mouse_delta;

  inline void dispatch() {
    window_size.x = static_cast<float>(GetScreenWidth());
    window_size.y = static_cast<float>(GetScreenHeight());
    window_resized = IsWindowResized();
    window_focused = IsWindowFocused();

    mouse_position = GetMousePosition();
#if defined(PLATFORM_WEB)
    mouse_position.x *= window_size.x;
    mouse_position.y *= window_size.y;
#endif
    mouse_delta = GetMouseDelta();
  }
}
