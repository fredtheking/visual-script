#pragma once
#include <string>
#include <functional>
using std::string_view, std::function;
template<typename T> using cref = const T&;

namespace vs::utils {
  bool sAssert(
    bool condition,
    string_view message = "Undefined error",
    bool crash = false
  );

  bool cAssert(
    bool condition,
    string_view fail_msg,
    string_view success_msg = "",
    bool crash = false,
    cref<function<void()>> on_success = nullptr,
    cref<function<void()>> on_fail = nullptr
  );
}
