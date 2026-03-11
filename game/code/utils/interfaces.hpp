#pragma once
#include "pch.hpp"

namespace mcke {
  struct IInit {
    virtual ~IInit() = default;

    virtual void init() = 0;
  };

  struct IProcessable {
    virtual ~IProcessable() = default;

    virtual void process() = 0;
  };

  struct IDrawable {
    virtual ~IDrawable() = default;

    virtual void draw() = 0;
  };
}