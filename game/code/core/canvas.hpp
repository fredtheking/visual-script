#pragma once
#include "block.hpp"
#include "pch.hpp"
#include "render/render_pack.hpp"
#include "render/smart_render_texture.hpp"
#include "utils/interfaces.hpp"

namespace vs {
  class App;

  class Canvas : ITerminatable, IDrawableObject, public IProcessable, public IRenderable, public IDebugDrawable {
  public:
    static constexpr float GRID_SPACING = 20;
  private:
    Shader vignette_shader;
    int vignette_shader_color_uloc;
    Color vignette_color;
    Camera2D camera;
    float camera_zoom_target = 0;
    Vector2 camera_velocity = Vector2Zero();
    vector<Block> blocks;
    Rectangle visible_rect;

    void _init_camera();
    void _init_rl();
    void init();
    void _term_rl();
    void term() override;
    void _process_move_camera();
    void _process_visible_rect();
    void _draw_grid() const;
    void draw_world() const override;
    void draw_ui() const override;
  public:
    friend class Block;
    friend class Hitbox;
    Canvas(cref<Vector2> size);
    ~Canvas() final;

    void set_vignette_color(Color color);
    [[nodiscard]] Vector2 get_size() const;
    void reset_camera();
    void process() override;
    [[nodiscard]] RenderPack render() const override;
    void draw_debug() const override;
  };
}
