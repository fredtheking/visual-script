#pragma once
#include "pch.hpp"
#include "utils/interfaces.hpp"

namespace vs {
  class Hitbox : public IWorldTypeObject, public IDebugDrawable {
    Rectangle* parent;

  public:
    Hitbox(WorldType type, Rectangle* parent);
    ~Hitbox() override = default;

    void draw_debug() override;
  };
}
