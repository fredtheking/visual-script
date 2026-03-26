#pragma once
#include "pch.hpp"
#include "render/smart_render_texture.hpp"
#include "utils/rectangle_manip.hpp"

namespace vs {
  struct RenderPack {
    explicit RenderPack(cref<RenderTexture2D> rt)
      : target(rt)
      , source(RectangleFromSize({ (float)rt.texture.width, -(float)rt.texture.height }))
    {}
    explicit RenderPack(cref<SmartRenderTexture> rrt)
      : target(rrt.texture)
      , source(RectangleFromSize({ (float)rrt.texture.texture.width, -(float)rrt.texture.texture.height }))
    {}
    explicit RenderPack()
      : target({})
      , source(RectangleZero())
    {}

    RenderTexture2D target;
    Rectangle source;
    optional<Vector2> preposition = nullopt;

    [[nodiscard]] bool valid() const {
      return IsRenderTextureValid(target)
      and
      not RectangleEquals(source, RectangleZero());
    }
    cref<RenderPack> with_pre_position(cref<Vector2> position) {
      preposition = position;
      return *this;
    }
    void draw(maybe<Vector2> position, maybe<Vector2> size) const {
      if (not valid()) return;

      auto [x, y] = position.has_value() ? position.value() : (preposition.has_value() ? preposition.value() : Vector2Zero());
      auto [width, height] = size.has_value() ? size.value() : Vector2{source.width, source.height};
      const Rectangle destination{ x, y, width, height };

      SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
      DrawTexturePro(target.texture, source, destination, Vector2Zero(), 0, WHITE);
    }
  };
}
