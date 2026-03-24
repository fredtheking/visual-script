#pragma once
#include "hitbox.hpp"
#include "pch.hpp"
#include "utils/interfaces.hpp"
#include "utils/rectangle_manip.hpp"

namespace vs {
  class Canvas;

  class Block : public ITerminatable, public IProcessable, public IDrawableObject, public IDebugDrawable {
  public:
    static constexpr float HEADER_SIZE = 20;
  private:
    Rectangle header_rect_offset;
    Hitbox header_hitbox;
    Rectangle content_rect_offset;
    Rectangle footer_rect_offset;
    Hitbox footer_hitbox;
    Rectangle rectangle;
    Canvas* canvas;

    void init();
    void term() override;
  public:
    Block(Canvas* canvas, cref<Rectangle> content_rect);
    ~Block() override;

    void process() override;
    void draw_world() const override;
    void draw_ui() const override {}
    void draw_debug() const override;
  };
}
