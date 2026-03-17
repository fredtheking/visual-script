#pragma once
#include "pch.hpp"
#include "render/render_pack.hpp"
#include "render/resizable_render_texture_2d.hpp"
#include "utils/interfaces.hpp"

namespace vs {
  class App;

  class Canvas : ITerminatable, IDrawableObject, public IProcessable, public IRenderable {
    ResizableRenderTexture2D rt;
    Camera2D camera;
    float camera_zoom_target = 0;
    Vector2 camera_velocity = Vector2Zero();

    void _init_camera();
    void init();
    void term() override;
    void _move_camera();
    void _draw_grid() const;
    void draw_world() override;
    void draw_ui() override;
  public:
    Canvas(cref<Vector2> size);
    ~Canvas() final;

    void resize(cref<Vector2> size);
    [[nodiscard]] Vector2 get_size() const;
    void reset_camera();
    void process() override;
    RenderPack render() override;
  };
}
