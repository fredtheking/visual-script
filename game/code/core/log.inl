#pragma once

template<typename... Args>
void Log::print(FORMAT_ARGS) {
  print(utils::format(fmt, std::forward<Args>(args)...));
}
template<typename... Args>
void Log::println(FORMAT_ARGS) {
  println(utils::format(fmt, std::forward<Args>(args)...));
}
template<typename... Args>
void Log::error(FORMAT_ARGS) {
  error(utils::format(fmt, std::forward<Args>(args)...));
}
template<typename... Args>
void Log::ok(FORMAT_ARGS) {
  ok(utils::format(fmt, std::forward<Args>(args)...));
}
template<typename... Args>
void Log::info(FORMAT_ARGS) {
  info(utils::format(fmt, std::forward<Args>(args)...));
}
template<typename... Args>
void Log::warning(FORMAT_ARGS) {
  warning(utils::format(fmt, std::forward<Args>(args)...));
}

template<typename... Args>
void Log::_separator(maybe<Color> bg, maybe<Color> fg, FORMAT_ARGS) {
  _separator(bg, fg, utils::format(fmt, std::forward<Args>(args)...));
}
