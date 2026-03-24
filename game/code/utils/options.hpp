#pragma once
#include "frame.hpp"
#include "pch.hpp"

namespace vs::options {
  namespace startup {
    inline string window_title;
    inline Vector2 window_size;
    inline Vector2 screen_size;

    inline void set_window_title(const string_view new_title) {
      window_title = new_title.data();
    }
    inline void set_window_size(const float x, const float y) {
      window_size = Vector2{ x, y };
    }

    inline void _setup() {
      set_window_title("Visual-Script");
#if defined(PLATFORM_WEB)
      set_window_size(1, 1);
#else
      set_window_size(800, 600);
#endif
    }
  }

  inline void _setup() {
    startup::_setup();
  }
}
