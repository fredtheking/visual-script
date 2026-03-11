#include "app.hpp"
#include "log.hpp"

namespace vs {
  void App::_init_rl() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_ALWAYS_RUN);

    InitWindow(1920, 1080, "Visual Script");
    InitAudioDevice();
  }

  App* App::init(const int argc, char* argv[]) {
    auto* game = new App(argc, argv);

    game->_init_rl();

    return game;
  }

  void App::loop() {
    while (not WindowShouldClose()) {
      Vector2 size = {(float)GetScreenWidth(), (float)GetScreenHeight()};


      BeginDrawing();
      ClearBackground(BLACK);

      DrawRectangleLinesEx({0, 0, size.x, size.y}, 9, RED);

      DrawFPS(24, 20);
      EndDrawing();
    }
  }


  int App::term() {
    CloseAudioDevice();
    CloseWindow();

    delete this;

    return EXIT_SUCCESS;
  }

  int App::$run(const int argc, char *argv[]) {
    auto* game = init(argc, argv);

    game->loop();

    return game->term();
  }
}
