#pragma once
#include "pch.hpp"
#include "utils/frame.hpp"
#include "utils/interfaces.hpp"

namespace vs {
  class Canvas;

  class Hitbox : public IWorldTypeObject, public IProcessable, public IDebugDrawable {
    Rectangle rectangle;
    Canvas* canvas;
    bool _hover;
    bool _press[MOUSE_BUTTON_COUNT];
    bool _hold[MOUSE_BUTTON_COUNT];
    bool _drag[MOUSE_BUTTON_COUNT];

    [[nodiscard]] Camera2D* _get_camera_2d() const override;
    [[nodiscard]] bool drag_v(MouseButton button) const;
  public:
    Hitbox();
    Hitbox(WorldType type, cref<Rectangle> rectangle, Canvas* canvas = nullptr);
    ~Hitbox() override = default;

    Color debug_color = RED;

    [[nodiscard]] bool hover() const;
    [[nodiscard]] bool press(MouseButton button) const;
    [[nodiscard]] bool hold(MouseButton button) const;
    [[nodiscard]] bool drag(MouseButton button) const;
    void process() override;
    void draw_debug() const override;
  };
}
