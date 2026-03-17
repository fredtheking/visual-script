#pragma once
#include "pch.hpp"
#include "utils/rectangle_manip.hpp"

namespace vs {
  struct RenderPack {
    explicit RenderPack(cref<RenderTexture2D> rt)
      : target(rt)
      , source(RectangleSize({ (float)rt.texture.width, -(float)rt.texture.height }))
    {}
    explicit RenderPack()
      : target({})
      , source(RectangleZero())
    {}

    RenderTexture2D target;
    Rectangle source;

    [[nodiscard]] bool valid() const {
      return IsRenderTextureValid(target)
      and
      not RectangleEquals(source, RectangleZero());
    }
    void draw(maybe<Vector2> position, maybe<Vector2> size) const {
      if (not valid()) return;

      auto [x, y] = position.has_value() ? position.value() : Vector2Zero();
      auto [width, height] = size.has_value() ? size.value() : Vector2{source.width, source.height};
      const Rectangle destination{ x, y, width, height };

      SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
      DrawTexturePro(target.texture, source, destination, Vector2Zero(), 0, WHITE);
    }
  };
}
