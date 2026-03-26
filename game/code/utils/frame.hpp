#pragma once
#include "pch.hpp"
#include "core/log.hpp"

#define MOUSE_BUTTON_COUNT (MOUSE_BUTTON_BACK+1)

namespace vs::frame {
  inline Vector2 window_size;
  inline bool window_maximized;
  inline bool window_resized;
  inline bool window_focused;

  inline Vector2 mouse_position;
  inline Vector2 mouse_delta;
  inline bool mouse_pressed[MOUSE_BUTTON_COUNT];
  inline bool mouse_down[MOUSE_BUTTON_COUNT];
  inline bool mouse_released[MOUSE_BUTTON_COUNT];

  inline bool debug_mode = true;
  inline int fps;

  inline void dispatch() {
    if (IsKeyPressed(KEY_F11))
      window_maximized ? RestoreWindow() : MaximizeWindow();

    window_size.x = static_cast<float>(GetScreenWidth());
    window_size.y = static_cast<float>(GetRenderHeight());
    window_maximized = IsWindowMaximized();
    window_resized = IsWindowResized();
    window_focused = IsWindowFocused();

    mouse_position = GetMousePosition();
#if defined(PLATFORM_WEB)
    mouse_position.x *= window_size.x;
    mouse_position.y *= window_size.y;
#endif
    mouse_delta = GetMouseDelta();
    for (byte i = 0; i < MOUSE_BUTTON_COUNT; i++) {
      mouse_pressed[i] = IsMouseButtonPressed(i);
      mouse_down[i] = IsMouseButtonDown(i);
      mouse_released[i] = IsMouseButtonReleased(i);
    }

    if (IsKeyPressed(KEY_D))
      debug_mode = not debug_mode;

    fps = GetFPS();
  }
}
