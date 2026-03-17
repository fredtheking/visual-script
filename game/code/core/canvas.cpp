#include "canvas.hpp"
#include "utils/frame.hpp"
#include "utils/options.hpp"
#include "utils/string_manip.hpp"
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
    static Vector2 last_mouse_pos;

    const Vector2 mouse_now = GetScreenToWorld2D(frame::mouse_position, camera);

    if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) {
      last_mouse_pos = mouse_now;
      camera_velocity = Vector2Zero();
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
      const Vector2 diff = last_mouse_pos - mouse_now;

      camera.target += diff;
      camera_velocity = diff;

      last_mouse_pos = GetScreenToWorld2D(frame::mouse_position, camera);
    } else {
      camera.target += camera_velocity;
      camera_velocity *= 0.87f;

      if (Vector2Length(camera_velocity) < 0.01f) camera_velocity = Vector2Zero();
    }

    if (not IsKeyDown(KEY_LEFT_CONTROL))
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
    Rectangle rec = RectangleSize(options::window_size / 2);
    const auto color = ColorAlpha(LIME, 0.1);

    DrawRectangleGradientEx(rec, color, color, BLANK, color);
    DrawRectangleGradientEx(RectangleAddPos(rec, {rec.width, 0}), color, BLANK, color, color);
    DrawRectangleGradientEx(RectangleAddPos(rec, {0, rec.height}), color, color, color, BLANK);
    DrawRectangleGradientEx(RectangleAddPos(rec, {rec.width, rec.height}), BLANK, color, color, color);
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
