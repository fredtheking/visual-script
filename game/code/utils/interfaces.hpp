#pragma once
#include "pch.hpp"

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

  struct IRenderable {
    virtual ~IRenderable() = default;

    virtual RenderPack render() = 0;
  };
}