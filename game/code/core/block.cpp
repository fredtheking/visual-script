#include "block.hpp"

#include "canvas.hpp"
#include "render/render_pack.hpp"
#include "utils/interfaces.hpp"
#include "utils/options.hpp"
#include "utils/rectangle_manip.hpp"

namespace vs {
  unordered_map<Block::BlockType, RenderPack> Block::rendered = {};

  void Block::init() {
    const Vector2 rec_pos = RectangleGetPos(rectangle);

    header_hitbox = Hitbox(Hitbox::WorldType::D2, RectangleAddPos(header_rect_offset, rec_pos), canvas);
    footer_hitbox = Hitbox(Hitbox::WorldType::D2, RectangleAddPos(footer_rect_offset, rec_pos), canvas);

    if (not rendered.contains(type))
      rendered[type] = render();
  }
  void Block::term() {

  }

  RenderPack Block::render() const {
    const SmartRenderTexture render_texture(RectangleGetSize(rectangle), true);
    render_texture.begin_texture_mode(BLACK);

    DrawRectangleRec(header_rect_offset, utils::DIMGRAY);
    DrawRectangleLinesEx(header_rect_offset, 1, GRAY);
    DrawRectangleRec(content_rect_offset, BLACK);
    DrawRectangleLinesEx(content_rect_offset, 1, GRAY);
    DrawRectangleRec(footer_rect_offset, utils::DIMGRAY);
    DrawRectangleLinesEx(footer_rect_offset, 1, GRAY);

    DrawRectangleLinesEx(RectangleFromSize(RectangleGetSize(rectangle)), 2, GRAY);

    render_texture.end_texture_mode();
    return RenderPack(render_texture);
  }

  Block::Block(Canvas* canvas, cref<Rectangle> content_rect)
    : header_rect_offset(RectangleFromSize({content_rect.width, HEADER_SIZE}))
    , content_rect_offset(RectangleFromVectors(Vector2UnitY * header_rect_offset.height, RectangleGetSize(content_rect)))
    , footer_rect_offset(RectangleAddPos(header_rect_offset, Vector2UnitY * (header_rect_offset.height + content_rect_offset.height)))
    , rectangle(RectangleFromVectors(RectangleGetPos(content_rect), RectangleGetSize(header_rect_offset) + Vector2UnitY * content_rect.height + Vector2UnitY * footer_rect_offset.height))
    , canvas(canvas)
    , type(BlockType::DEFAULT_NONE)
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
    if (CheckCollisionRecs(canvas->visible_rect, rectangle))
      rendered[type].draw(RectangleGetPos(rectangle), RectangleGetSize(rectangle));
  }

  void Block::draw_debug() const {
    header_hitbox.draw_debug();
    footer_hitbox.draw_debug();
  }
}
