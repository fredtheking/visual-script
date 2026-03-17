#include "canvas.hpp"

#include "utils/frame.hpp"
#include "utils/options.hpp"
#include "utils/utils.hpp"

namespace vs {
  Canvas::Canvas(cref<Vector2> size)
  : rt(size)
  , camera(Vector2Zero(), Vector2Zero(), 0, 1)
  { init(); }
  Canvas::~Canvas() { Canvas::term(); }

  void Canvas::resize(cref<Vector2> size) {
    rt.resize(size);
  }

  Vector2 Canvas::get_size() const {
    return rt.get_size();
  }

  void Canvas::reset_camera() {
    camera.target = Vector2Negate(get_size() / 2);
    camera.offset = Vector2Zero();
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    camera_zoom_target = 1.0f;
  }

  void Canvas::_init_camera() {
    reset_camera();
  }
  void Canvas::init() {
    _init_camera();
  }

  void Canvas::term() {

  }

  void Canvas::_move_camera() {
    static Vector2 last_mouse_world_pos;
    static bool dragging;

    const Vector2 mouse_world_now = GetScreenToWorld2D(frame::mouse_position, camera);

    if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) {
      last_mouse_world_pos = mouse_world_now;
      dragging = true;
      camera_velocity = Vector2Zero();
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
      const Vector2 diff = last_mouse_world_pos - mouse_world_now;

      camera.target += diff;
      camera_velocity = diff;

      last_mouse_world_pos = GetScreenToWorld2D(frame::mouse_position, camera);
    } else {
      dragging = false;

      camera.target += camera_velocity;
      camera_velocity *= 0.87f;

      if (std::abs(camera_velocity.x) < 0.01f) camera_velocity.x = 0;
      if (std::abs(camera_velocity.y) < 0.01f) camera_velocity.y = 0;
    }

    if (const float wheel = GetMouseWheelMove(); wheel != 0) {
      const Vector2 mouse_world_pos = GetScreenToWorld2D(frame::mouse_position, camera);
      camera.offset = frame::mouse_position;
      camera.target = mouse_world_pos;
      camera_zoom_target += wheel * 0.12f;
    }

    camera_zoom_target = std::clamp<float>(camera_zoom_target, 0.5, 3.0);
    camera.zoom = std::lerp(camera.zoom, camera_zoom_target, 0.25f);
  }

  void Canvas::_draw_grid() const {
    static constexpr float GRID_SPACING = 20;
    static constexpr float OUTER_SPACE = 1;
    static const auto GRID_COLOR = ColorAlpha(WHITE, 0.2);

    auto [x1, y1] = GetScreenToWorld2D(Vector2Zero(), camera);
    auto [x2, y2] = GetScreenToWorld2D(get_size(), camera);

    constexpr float intensity = 0.5;
    const auto time = static_cast<float>(GetTime() / 2.0);

    const auto start_x = std::floor(x1 / GRID_SPACING + OUTER_SPACE) * GRID_SPACING;
    const auto end_x   = std::ceil(x2  / GRID_SPACING - OUTER_SPACE) * GRID_SPACING;

    const auto start_y = std::floor(y1 / GRID_SPACING + OUTER_SPACE) * GRID_SPACING;
    const auto end_y   = std::ceil(y2  / GRID_SPACING - OUTER_SPACE) * GRID_SPACING;

    for (float x = start_x; x <= end_x; x += GRID_SPACING) {
      const float offset = std::sin(time * 2.0f + x * 0.05f) * intensity;
      DrawLine(
        x + offset, y1,
        x + offset, y2,
        GRID_COLOR
      );
    }

    for (float y = start_y; y <= end_y; y += GRID_SPACING) {
      const float offset = std::sin(time * 1.2f + y * 0.05f) * intensity;
      DrawLine(
        x1, y + offset,
        x2, y + offset,
        GRID_COLOR
      );
    }

    DrawLine(0.0f, y1, 0.0f, y2, ColorAlpha(BLUE, 0.89));
    DrawLine(x1, 0.0f, x2, 0.0f, ColorAlpha(RED, 0.89));
  }

  void Canvas::draw_world() {
    BeginMode2D(camera);

    _draw_grid();
    DrawTextEx(GetFontDefault(), "Hello, world!", {200, 100}, 42, 4, WHITE);

    EndMode2D();
  }
  void Canvas::draw_ui() {
    DrawTextEx(GetFontDefault(), "Hello, ui!", {200, 400}, 42, 1, RAYWHITE);
  }

  void Canvas::process() {
    if (IsKeyPressed(KEY_R)) reset_camera();
    _move_camera();
  }

  RenderPack Canvas::render() {
    rt.begin_texture_mode(utils::DIMBLACK);

    draw_world();
    draw_ui();

    ResizableRenderTexture2D::end_texture_mode();
    return RenderPack(rt.texture);
  }
}
