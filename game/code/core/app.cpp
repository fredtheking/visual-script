#include "app.hpp"
#include "canvas.hpp"
#include "utils/options.hpp"
#include "log.hpp"
#include "utils/frame.hpp"

namespace vs {
  void App::_begin() {
    frame::dispatch();
    options::dispatch();
  }
  void App::_after_proc() {
    BeginDrawing();
    ClearBackground(BLACK);
  }
  void App::_end() {
    DrawFPS(10, 10);
    EndDrawing();
  }

  void App::_init_rl() {
    SetConfigFlags(
      FLAG_WINDOW_RESIZABLE |
      FLAG_VSYNC_HINT |
      FLAG_WINDOW_ALWAYS_RUN);

    InitWindow(
      options::window_size.x,
      options::window_size.y,
      options::window_title.c_str()
    );
    InitAudioDevice();
  }

  App* App::init(const int argc, char* argv[]) {
    auto* app = new App(argc, argv);

    options::_setup();
    app->_init_rl();
    app->canvas = new Canvas(options::window_size);

    return app;
  }

  void App::loop() {
    while (not WindowShouldClose()) {
      _begin();
      _process();
      _after_proc();
      _draw();
      _end();
    }
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
        canvas->resize(options::window_size);
    }
  }

  void App::_draw_unfocused_caution() {
    static constexpr float BG_ALPHA = 0.4;

    const auto rec = RectangleSize(options::window_size);
    DrawRectangleRec(rec, ColorAlpha(BLACK, BG_ALPHA));
    DrawRectangleLinesEx(rec, 16, RED);
    DrawRectangleLinesEx(rec, 6, ORANGE);

    const auto text = "Unfocused window is not rendered";
    constexpr float fontsize = 42;
    const auto size_x = static_cast<float>(MeasureText(text, fontsize));
    DrawText(
      text,
      static_cast<int>(std::round(options::window_size.x - size_x) / 2.0),
      static_cast<int>(std::round(options::window_size.y - fontsize / 2) / 2.0),
      fontsize, WHITE);
  }

  void App::_draw() {
    if (canvas) {
      static RenderPack latest_canvas_rp;

      if (frame::window_focused) latest_canvas_rp = canvas->render();
      latest_canvas_rp.draw(nullopt, nullopt);
    }

    if (not frame::window_focused) _draw_unfocused_caution();
  }

  int App::$run(const int argc, char *argv[]) {
    auto* game = init(argc, argv);

    game->loop();

    return game->term();
  }
}
