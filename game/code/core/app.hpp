#pragma once
#include "pch.hpp"
#include "render/render_pack.hpp"
#include "utils/interfaces.hpp"

namespace vs {
  class Canvas;

  class App : IPrivProcessable, IPrivDrawable {
    App(int argc, char* argv[]) {}
    ~App() final = default;

    Canvas* canvas;

    void _begin();
    void _after_proc();
    void _end();
    void _init_rl();
    static App* init(int argc, char* argv[]);
    void loop();
    void _term_rl();
    int term();
    void _process() override;
    static void _draw_unfocused_caution();
    void _draw() override;
  public:
    void _while_chunk();
    static int $run(int argc, char* argv[]);
  };
}
