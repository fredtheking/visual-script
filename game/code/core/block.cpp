#include "block.hpp"

#include "canvas.hpp"
#include "render/render_pack.hpp"
#include "utils/interfaces.hpp"
#include "utils/rectangle_manip.hpp"

namespace vs {
  void Block::init() {
    const Vector2 rec_pos = RectangleGetPos(rectangle);

    header_hitbox = Hitbox(Hitbox::WorldType::D2, RectangleAddPos(header_rect_offset, rec_pos), canvas);
    footer_hitbox = Hitbox(Hitbox::WorldType::D2, RectangleAddPos(footer_rect_offset, rec_pos), canvas);
  }
  void Block::term() {

  }

  Block::Block(Canvas* canvas, cref<Rectangle> content_rect)
    : header_rect_offset(RectangleFromSize({content_rect.width, HEADER_SIZE}))
    , content_rect_offset(RectangleFromVectors(Vector2UnitY * header_rect_offset.height, RectangleGetSize(content_rect)))
    , footer_rect_offset(RectangleAddPos(header_rect_offset, Vector2UnitY * (header_rect_offset.height + content_rect_offset.height)))
    , rectangle(RectangleFromVectors(RectangleGetPos(content_rect), RectangleGetSize(header_rect_offset) + Vector2UnitY * content_rect.height + Vector2UnitY * footer_rect_offset.height))
    , canvas(canvas)
  { init(); }
  Block::~Block() { Block::term(); }

  void Block::process() {
    header_hitbox.process();
    footer_hitbox.process();

    if (header_hitbox.drag(MOUSE_BUTTON_LEFT)) {
      Vector2 delta = frame::mouse_delta;
      delta.x /= canvas->camera.zoom;
      delta.y /= canvas->camera.zoom;
      rectangle = RectangleAddPos(rectangle, delta);
    }
    footer_hitbox.debug_color = footer_hitbox.hover() ? BLUE : RED;
  }

  void Block::draw_world() const {
    const Vector2 rec_pos = RectangleGetPos(rectangle);

    const Rectangle header = RectangleAddPos(header_rect_offset, rec_pos);
    const Rectangle content = RectangleAddPos(content_rect_offset, rec_pos);
    const Rectangle footer = RectangleAddPos(footer_rect_offset, rec_pos);

    DrawRectangleRec(header, utils::DIMGRAY);
    DrawRectangleLinesEx(header, 1, GRAY);
    DrawRectangleRec(content, BLACK);
    DrawRectangleLinesEx(content, 1, GRAY);
    DrawRectangleRec(footer, utils::DIMGRAY);
    DrawRectangleLinesEx(footer, 1, GRAY);

    DrawRectangleLinesEx(rectangle, 2, GRAY);
  }

  void Block::draw_debug() const {
    header_hitbox.draw_debug();
    footer_hitbox.draw_debug();
  }
}
