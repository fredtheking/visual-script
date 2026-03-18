#pragma once
#include "pch.hpp"
#include "utils/interfaces.hpp"

namespace vs {
  class Block : IDrawableObject {
    Vector2 position;
  public:
    void draw_world() override;
    void draw_ui() override {}
  };
}
