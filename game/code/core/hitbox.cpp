#include "hitbox.hpp"

namespace vs {
  Hitbox::Hitbox(const WorldType type, Rectangle* parent)
    : IWorldTypeObject(type)
    , parent(parent)
  {}

  void Hitbox::draw_debug() {

  }
}
