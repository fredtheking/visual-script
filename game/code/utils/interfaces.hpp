#pragma once
#include "pch.hpp"
#include "core/log.hpp"
#include "render/resizable_render_texture_2d.hpp"

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

    virtual void draw_world() const = 0;
    virtual void draw_ui() const = 0;
  };
  struct IDrawable {
    virtual ~IDrawable() = default;

    virtual void draw() const = 0;
  };
  struct IPrivDrawable {
    virtual ~IPrivDrawable() = default;

    virtual void _draw() const = 0;
  };
  struct IDebugDrawable {
    virtual ~IDebugDrawable() = default;

    virtual void draw_debug() const = 0;
  };

  struct IRenderable {
  protected:
    ResizableRenderTexture2D main;
    ResizableRenderTexture2D scratch;
  public:
    explicit IRenderable(cref<Vector2> size) : main(size), scratch(size) {}
    virtual ~IRenderable() = default;

    virtual void resize(cref<Vector2> size) {
      main.resize(size);
      scratch.resize(size);
    }

    [[nodiscard]] virtual RenderPack render() const = 0;
  };

  struct IWorldTypeObject {
    virtual ~IWorldTypeObject() = default;

    enum class WorldType {
      UI,
      D2,
      UNDEFINED,
    };
  protected:
    explicit IWorldTypeObject(const WorldType type): type(type) {}

    WorldType type = WorldType::UI;

    [[nodiscard]] virtual Camera2D* _get_camera_2d() const { return nullptr; }
    virtual Vector2 getpos(cref<Vector2> pos) {
      switch (type) {
        case WorldType::UNDEFINED:
        default:
          Log::warning("Invalid object world type: its neither UI nor D2!");
          return Vector2Zero();

        case WorldType::UI:
          return pos;
        case WorldType::D2:
          const Camera2D* _camera = _get_camera_2d();

          if (_camera)
            return GetScreenToWorld2D(pos, *_camera);

          Log::warning("Camera2D is not overridden - unable to get valid pos");
          return Vector2Zero();
      }
    }
  };
}
