#pragma once
#include "pch.hpp"
#include "utils.hpp"

namespace vs {
  inline string Vector2String(cref<Vector2> v) {
    return utils::format("{ {}; {} }", v.x, v.y);
  }
  inline string RectangleString(cref<Rectangle> r) {
    return utils::format("{ {}; {} | {}; {} }", r.x, r.y, r.width, r.height);
  }
}
