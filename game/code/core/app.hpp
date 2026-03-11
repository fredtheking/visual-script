#pragma once
#include "pch.hpp"

namespace vs {
  class App {
    App(int argc, char* argv[]) {}
    ~App() = default;

    void _init_rl();
    static App* init(int argc, char* argv[]);
    void loop();
    int term();
  public:
    static int $run(int argc, char* argv[]);
  };
}