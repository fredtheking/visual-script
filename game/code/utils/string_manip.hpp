#pragma once
#include "pch.hpp"

#define FORMAT_ARGS string_view fmt, Args&&... args

namespace vs::utils {
  template<typename... Args>
  static string format(FORMAT_ARGS) {
    ostringstream oss;
    size_t argIndex = 0;
    string argStrs[] = {
      ([&]{
        ostringstream tmp;
        tmp << args;
        return tmp.str();
      }())...
    };

    for (size_t i = 0; i < fmt.size(); ++i) {
      if (fmt[i] == '{' && i+1 < fmt.size() && fmt[i+1] == '}') {
        oss << argStrs[argIndex++];
        ++i;
      } else
        oss << fmt[i];
    }
    return oss.str();
  }
}
