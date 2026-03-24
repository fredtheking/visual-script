#include "app.hpp"
#include "canvas.hpp"
#include "utils/frame.hpp"
#include "utils/options.hpp"

static Vector2 _get_canvas_size() {
  return vs::frame::window_size - (Vector2UnitY * 40);
}

namespace vs {
  void App::_begin() {
    frame::dispatch();
  }
  void App::_after_proc() {
    BeginDrawing();
    ClearBackground(BLACK);
  }
  void App::_end() {
    DrawFPS(10, 10);
    if (frame::debug_mode)
      DrawText("DEBUG", 120, 10, 20, PINK);
    EndDrawing();
  }

  void App::_init_rl() {
    int flags = FLAG_WINDOW_RESIZABLE
    | FLAG_VSYNC_HINT
    ;

    #if not defined(PLATFORM_WEB)
    flags |= FLAG_WINDOW_ALWAYS_RUN;
    #endif

    SetConfigFlags(flags);

    InitWindow(
      options::startup::window_size.x,
      options::startup::window_size.y,
      options::startup::window_title.c_str()
    );
    InitAudioDevice();
  }

  App* App::init(const int argc, char* argv[]) {
    auto* app = new App(argc, argv);

    options::_setup();
    app->_init_rl();
    frame::dispatch();
    app->canvas = new Canvas(_get_canvas_size());

    return app;
  }

  static void wasm_while(void* arg) {
    static_cast<App*>(arg)->_while_chunk();
  }
  void App::loop() {
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop_arg(wasm_while, this, 0, 1);
#else
    while (not WindowShouldClose()) _while_chunk();
#endif
  }

  void App::_term_rl() {
    CloseAudioDevice();
    CloseWindow();
  }
  int App::term() {
    _term_rl();

    delete this;

    return EXIT_SUCCESS;
  }

  void App::_process() {
    if (canvas) {
      canvas->process();
      if (frame::window_resized)
        canvas->resize(_get_canvas_size());
    }
  }

  void App::_draw_unfocused_caution() {
    static constexpr float BG_ALPHA = 0.4;

    const auto rec = RectangleFromSize(frame::window_size);
    DrawRectangleRec(rec, ColorAlpha(BLACK, BG_ALPHA));
    DrawRectangleLinesEx(rec, 16, RED);
    DrawRectangleLinesEx(rec, 6, ORANGE);

    const auto text = "Unfocused window is not rendered";
    constexpr float fontsize = 42;
    const auto size_x = static_cast<float>(MeasureText(text, fontsize));
    DrawText(
      text,
      static_cast<int>(std::round(frame::window_size.x - size_x) / 2.0),
      static_cast<int>(std::round(frame::window_size.y - fontsize / 2) / 2.0),
      fontsize, WHITE);
  }

  void App::_draw() const {
    if (canvas) {
      static RenderPack latest_canvas_rp;

      if (frame::window_focused) latest_canvas_rp = canvas->render();
      latest_canvas_rp.draw(nullopt, nullopt);
      canvas->draw_debug();
    }

    if (not frame::window_focused) _draw_unfocused_caution();
  }

  void App::_while_chunk() {
    _begin();
    _process();
    _after_proc();
    _draw();
    _end();
  }

  int App::$run(const int argc, char *argv[]) {
    auto* game = init(argc, argv);

    game->loop();

    return game->term();
  }
}
