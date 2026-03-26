#include "canvas.hpp"
#include "utils/frame.hpp"
#include "utils/utils.hpp"

namespace vs {
  Canvas::Canvas(cref<Vector2> size)
  : IRenderable(size)
  , vignette_shader()
  , camera(Vector2Zero(), Vector2Zero(), 0, 1)
  { init(); }
  Canvas::~Canvas() { Canvas::term(); }

  void Canvas::set_vignette_color(const Color color) {
    vignette_color = color;
    const auto normalized_color = ColorNormalize(color);
    SetShaderValue(vignette_shader, vignette_shader_color_uloc, &normalized_color, SHADER_UNIFORM_VEC4);
  }

  Vector2 Canvas::get_size() const {
    return main.get_size();
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
  void Canvas::_init_rl() {
    vignette_shader = LoadShader(nullptr, "resources/vignette.fs");
    vignette_shader_color_uloc = GetShaderLocation(vignette_shader, "colVignette");
    set_vignette_color(LIME);
  }
  void Canvas::init() {
    _init_camera();
    _init_rl();
  }

  void Canvas::_term_rl() {
    UnloadShader(vignette_shader);
  }
  void Canvas::term() {
    _term_rl();
  }

  void Canvas::_process_move_camera() {
    static Vector2 last_mouse_pos;

    const Vector2 mouse_now = GetScreenToWorld2D(frame::mouse_position, camera);

    const bool ctrl_down = IsKeyDown(KEY_LEFT_CONTROL);
    if (not ctrl_down) {
      if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)){
        last_mouse_pos = mouse_now;
        camera_velocity = Vector2Zero();
      }

      if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
        const Vector2 diff = last_mouse_pos - mouse_now;

        camera.target += diff;
        camera_velocity = diff;

        last_mouse_pos = GetScreenToWorld2D(frame::mouse_position, camera);
      }

      if (const float wheel = GetMouseWheelMove(); wheel != 0) {
        const Vector2 mouse_world_pos = GetScreenToWorld2D(frame::mouse_position, camera);
        camera.offset = frame::mouse_position;
        camera.target = mouse_world_pos;
        camera_zoom_target += wheel * 0.07f;
      }
    } else {
      int sensitivity = 252;
      Vector2 ctrl_move = Vector2Normalize(GetMouseWheelMoveV());
      #if defined(PLATFORM_WEB)
      ctrl_move.x = -ctrl_move.x;
      sensitivity = 100;
      #endif
      printf("shit - %s\n", Vector2String(ctrl_move).c_str());
      camera_velocity -= ctrl_move * GetFrameTime() * sensitivity;
    }

    if (IsMouseButtonUp(MOUSE_MIDDLE_BUTTON) or ctrl_down) {
      camera.target += camera_velocity;
      camera_velocity *= 0.87f;
      if (Vector2Length(camera_velocity) < 0.01f) camera_velocity = Vector2Zero();
    }

    static constexpr float CLAMP_MIN = 0.5;
    static constexpr float CLAMP_MAX = 3.0;

    camera_zoom_target = std::clamp<float>(camera_zoom_target, CLAMP_MIN, CLAMP_MAX);
    camera.zoom = frame::fps > 30 ? std::clamp(std::lerp(camera.zoom, camera_zoom_target, 25.2f * GetFrameTime()), CLAMP_MIN, CLAMP_MAX) : camera_zoom_target;
  }
  void Canvas::_process_visible_rect() {
    auto [tl_x, tl_y] = GetScreenToWorld2D(Vector2Zero(), camera);
    auto [br_x, br_y] = GetScreenToWorld2D(frame::window_size, camera);

    visible_rect = {
      tl_x,
      tl_y,
      br_x - tl_x,
      br_y - tl_y
    };
  }


  void Canvas::_draw_grid() const {
    static constexpr float OUTER_SPACE = 1;
    static const auto GRID_COLOR = ColorAlpha(WHITE, 0.5f);

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

  void Canvas::draw_world() const {
    BeginMode2D(camera);

    _draw_grid();
    for (cref<Block> block : blocks)
      block.draw_world();
    DrawTextEx(GetFontDefault(), "Hello, world!", {200, 100}, 42, 4, WHITE);

    EndMode2D();
  }
  void Canvas::draw_ui() const {
    for (cref<Block> block : blocks)
      block.draw_ui();

    DrawText(utils::format("Blocks count: {}", blocks.size()).c_str(), 10, 40, 20, RAYWHITE);
  }

  void Canvas::process() {
    if (IsKeyPressed(KEY_R)) reset_camera();
    _process_move_camera();

    _process_visible_rect();

    if (IsKeyDown(KEY_F) or IsKeyPressed(KEY_S)) {
      for (int i = 0; i <= 100 * IsKeyDown(KEY_F); ++i) {
        constexpr int SHIT = 1760;
        Vector2 pos = { (float)GetRandomValue(0, SHIT), (float)GetRandomValue(0, SHIT) };
        blocks.emplace_back(this, RectangleFromVectors(pos, {GRID_SPACING * 6, GRID_SPACING * 4}));
      }
    }

    for (Block& block : blocks)
      block.process();
  }

  RenderPack Canvas::render() const {
    scratch.begin_texture_mode(utils::DIMBLACK);
    draw_world();
    scratch.end_texture_mode();

    main.begin_texture_mode(BLANK);
    BeginShaderMode(vignette_shader);
    RenderPack{scratch.texture}.draw(nullopt, nullopt);
    EndShaderMode();
    main.end_texture_mode();

    scratch.begin_texture_mode(BLANK);
    draw_ui();
    scratch.end_texture_mode();

    main.begin_texture_mode(nullopt);
    RenderPack{scratch.texture}.draw(nullopt, nullopt);
    main.end_texture_mode();

    return RenderPack(main);
  }

  void Canvas::draw_debug() const {
    for (cref<Block> block : blocks)
      block.draw_debug();
  }
}
