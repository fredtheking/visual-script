#include "log.hpp"

namespace vs {
  string Log::_ansi_fg(cref<Color> color) {
    return "\033[38;2;" + to_string(color.r) + ";" + to_string(color.g) + ";" + to_string(color.b) + "m";
  }
  string Log::_ansi_fg(const byte r, const byte g, const byte b) {
    return _ansi_fg(Color{r, g, b});
  }

  string Log::_ansi_bg(cref<Color> color) {
    return "\033[48;2;" + to_string(color.r) + ";" + to_string(color.g) + ";" + to_string(color.b) + "m";
  }
  string Log::_ansi_bg(const byte r, const byte g, const byte b) {
    return _ansi_bg(Color{r, g, b});
  }

  string Log::_ansi_bold() {
    return "\033[1m";
  }

  string Log::_ansi_italic() {
    return "\033[3m";
  }

  string internal_ansiReset(const byte code = 0) {
    return "\033[" + to_string(code) + "m";
  }
  string Log::_ansi_reset(const _AnsiReset what) {
    byte code = 0;

    switch (what) {
      case _AnsiReset::ALL:
        code = 0; break;
      case _AnsiReset::FG:
        code = 39; break;
      case _AnsiReset::BG:
        code = 49; break;
      case _AnsiReset::BOLD:
        code = 22; break;
      case _AnsiReset::ITALIC:
        code = 23; break;
    }

    return internal_ansiReset(code);
  }

  enum class LogType {
    ERROR,

    OK,
    INFO,
    WARNING,
    CRASH,
  };
  string internal_logType(const LogType type) {
    ostringstream out;

    switch (type) {
      case LogType::ERROR:
        out
        << Log::_ansi_bg(RED)
        << Log::_ansi_fg(BLACK)
        << Log::_ansi_bold()
        << "   ERR "
        << Log::_ansi_reset(); break;
      case LogType::OK:
        out
        << Log::_ansi_bg(LIME)
        << Log::_ansi_fg(BLACK)
        << Log::_ansi_bold()
        << "    OK "
        << Log::_ansi_reset(); break;
      case LogType::INFO:
        out
        << Log::_ansi_bg(WHITE)
        << Log::_ansi_fg(BLACK)
        << Log::_ansi_bold()
        << "  INFO "
        << Log::_ansi_reset(); break;
      case LogType::WARNING:
        out
        << Log::_ansi_bg(YELLOW)
        << Log::_ansi_fg(BLACK)
        << Log::_ansi_bold()
        << "  WARN "
        << Log::_ansi_reset(); break;
      case LogType::CRASH:
        out
        << Log::_ansi_bg(BLACK)
        << Log::_ansi_fg(RED)
        << Log::_ansi_bold()
        << " CRASH "
        << Log::_ansi_reset(); break;
    }

    return out.str() + ' ';
  }

  void Log::print(const string_view msg) {
    std::cout << msg;
  }
  void Log::println(const string_view msg) {
    print(msg); std::cout << '\n';
  }
  void Log::error(const string_view msg) {
    std::cerr << internal_logType(LogType::ERROR) << msg << '\n';
  }

  void Log::ok(const string_view msg) {
    std::cout << internal_logType(LogType::OK) << msg << '\n';
  }
  void Log::info(const string_view msg) {
    std::cout << internal_logType(LogType::INFO) << msg << '\n';
  }
  void Log::warning(const string_view msg) {
    std::cout
    << internal_logType(LogType::WARNING)
    << _ansi_fg(BEIGE)
    << msg
    << '\n';
  }
  void Log::crash(cref<std::runtime_error> exception) {
    std::cerr << internal_logType(LogType::CRASH) << exception.what() << '\n';
    DEBUG_BREAK;
    throw exception;
  }


#if defined(_WIN32)
#include <windows.h>
  int internal_getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
      return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    return 80;
  }
#else
#include <sys/ioctl.h>
#include <unistd.h>
  int internal_getConsoleWidth() {
    struct winsize w;
    if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_col > 0)
      return w.ws_col;
    return 80;
  }
#endif
  string internal_separatorStr(const string_view msg) {
    const int width = internal_getConsoleWidth();
    const int msg_len = static_cast<int>(msg.size());
    const int dash_total = width - msg_len - 2;
    if(dash_total <= 0) return msg.data();

    const int left = dash_total / 2 - 1;
    const int right = dash_total - left - 1;

    ostringstream oss;
    oss << ' ' << string(left, '=') << ' ' << msg << ' ' << string(right, '=') << ' ';
    return oss.str();
  }
  void Log::_separator(maybe<Color> bg, maybe<Color> fg, const string_view msg) {
    const string formatted = internal_separatorStr(msg);

    ostringstream oss;

    if (bg.has_value()) oss << _ansi_bg(bg.value());
    if (fg.has_value()) oss << _ansi_fg(fg.value());
    oss << _ansi_bold() << formatted << _ansi_reset() << '\n';

    std::cout << oss.str();
  }
}
