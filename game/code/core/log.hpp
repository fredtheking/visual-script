#pragma once
#include "pch.hpp"
#include "utils/string_manip.hpp"

namespace vs {
  struct Log {
    enum class _AnsiReset { ALL, FG, BG, BOLD, ITALIC };

    static string _ansi_fg(cref<Color> color);
    static string _ansi_fg(byte r, byte g, byte b);

    static string _ansi_bg(cref<Color> color);
    static string _ansi_bg(byte r, byte g, byte b);

    static string _ansi_bold();
    static string _ansi_italic();

    static string _ansi_reset(_AnsiReset what = _AnsiReset::ALL);

    template<typename... Args>
    static void print(FORMAT_ARGS);
    static void print(string_view msg);
    template<typename... Args>
    static void println(FORMAT_ARGS);
    static void println(string_view msg);
    template<typename... Args>
    static void error(FORMAT_ARGS);
    static void error(string_view msg);

    template<typename... Args>
    static void ok(FORMAT_ARGS);
    static void ok(string_view msg);
    template<typename... Args>
    static void info(FORMAT_ARGS);
    static void info(string_view msg);
    template<typename... Args>
    static void warning(FORMAT_ARGS);
    static void warning(string_view msg);
    static void crash(cref<std::runtime_error> exception);

    template<typename... Args>
    static void _separator(maybe<Color> bg, maybe<Color> fg, FORMAT_ARGS);
    static void _separator(maybe<Color> bg, maybe<Color> fg, string_view msg);

    STATIC_ONLY_CONSTRUCTOR(Log)
  };

  #include "log.inl"
}
