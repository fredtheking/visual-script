#pragma once
#include "pch.hpp"
#include "core/log.hpp"

namespace vs {
  struct RenderPack;

  struct ITerminatable {
    virtual ~ITerminatable() = default;

    virtual void term() = 0;
  };

  struct IProcessable {
    virtual ~IProcessable() = default;

    virtual void process() = 0;
  };
  struct IPrivProcessable {
    virtual ~IPrivProcessable() = default;

    virtual void _process() = 0;
  };

  struct IDrawableObject {
    virtual ~IDrawableObject() = default;

    virtual void draw_world() = 0;
    virtual void draw_ui() = 0;
  };
  struct IDrawable {
    virtual ~IDrawable() = default;

    virtual void draw() = 0;
  };
  struct IPrivDrawable {
    virtual ~IPrivDrawable() = default;

    virtual void _draw() = 0;
  };
  struct IDebugDrawable {
    virtual ~IDebugDrawable() = default;

    virtual void draw_debug() = 0;
  };

  struct IRenderable {
    virtual ~IRenderable() = default;

    virtual RenderPack render() = 0;
  };

  struct IWorldTypeObject {
    virtual ~IWorldTypeObject() = default;

  protected:
    enum class WorldType {
      UI,
      D2,
    };
    explicit IWorldTypeObject(const WorldType type): type(type) {}

    WorldType type = WorldType::UI;

    virtual maybe<Camera2D> _get_camera_2d() { return nullopt; }
    virtual Vector2 getpos(cref<Vector2> pos) {
      switch (type) {
        case WorldType::UI:
          return pos;
        case WorldType::D2:
          maybe<Camera2D> _camera = _get_camera_2d();

          if (_camera.has_value())
            return GetScreenToWorld2D(pos, _camera.value());

          Log::warning("Camera2D is not overridden - unable to get valid pos");
          return Vector2Zero();
      }
      Log::warning("Invalid object world type: its neither UI nor D2!");
      return Vector2Zero();
    }
  };
}
