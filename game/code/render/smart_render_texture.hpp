#pragma once
#include "pch.hpp"

namespace vs {
  struct SmartRenderTexture {
    explicit SmartRenderTexture(cref<Vector2> size, bool preserve = false): texture(), preserve(preserve) { load(size); }
    ~SmartRenderTexture() { if (not preserve) unload(); }

    RenderTexture2D texture;
    bool preserve;

    void load(cref<Vector2> size) {
      if (not valid())
        texture = LoadRenderTexture(size.x, size.y);
    }
    void unload() {
      if (valid()) {
        UnloadRenderTexture(texture);
        texture = {};
      }
    }
    [[nodiscard]] bool valid() const { return IsRenderTextureValid(texture); }
    void resize(cref<Vector2> size) {
      unload();
      load(size);
    }
    [[nodiscard]] Vector2 get_size() const {
      return { (float)texture.texture.width, (float)texture.texture.height };
    }
    void begin_texture_mode(maybe<Color> color) const {
      BeginTextureMode(texture);
      if (color.has_value()) clear(color.value());
    }
    void clear(cref<Color> color) const {
      ClearBackground(color);
    }
    void end_texture_mode() const {
      EndTextureMode();
    }
  };
}