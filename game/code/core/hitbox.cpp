#include "hitbox.hpp"

#include "canvas.hpp"
#include "utils/options.hpp"
#include "utils/rectangle_manip.hpp"

namespace vs {
  Hitbox::Hitbox() : IWorldTypeObject(WorldType::UNDEFINED) {}
  Hitbox::Hitbox(const WorldType type, cref<Rectangle> rectangle, Canvas* canvas)
    : IWorldTypeObject(type)
    , rectangle(rectangle)
    , canvas(canvas)
    , _hover(false)
    , _press{false, false, false}
    , _hold{false, false, false}
    , _drag{false, false, false}
  {}

  Camera2D* Hitbox::_get_camera_2d() const {
    return &canvas->camera;
  }

  bool Hitbox::drag_v(const MouseButton button) const {
    if (not _drag[button] and _press[button]) return true;
    if (_drag[button] and not frame::mouse_down[button]) return false;
    return _drag[button];
  }

  bool Hitbox::hover() const { return _hover; }
  bool Hitbox::press(const MouseButton button) const { return _press[button]; }
  bool Hitbox::hold(const MouseButton button) const { return _hold[button]; }
  bool Hitbox::drag(const MouseButton button) const { return _drag[button]; }

  void Hitbox::process() {
    _hover = CheckCollisionPointRec(getpos(frame::mouse_position), rectangle);
    for (int i = 0; i < MOUSE_BUTTON_COUNT; i++) {
      _press[i] = frame::mouse_pressed[i] and _hover;
      _hold[i] = frame::mouse_down[i] and _hover;
      _drag[i] = drag_v(static_cast<MouseButton>(i));
    }
  }

  void Hitbox::draw_debug() const {
    if (frame::debug_mode)
      DrawRectangleRec(RectangleGetWorldToScreen(rectangle, *_get_camera_2d()), ColorAlpha(debug_color, 0.52));
  }
}
