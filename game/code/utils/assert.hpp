#pragma once
#include <string>
#include <functional>

namespace vs::utils {
  bool sAssert(
    bool condition,
    std::string_view message = "Undefined error",
    bool crash = false
  );

  bool cAssert(
    bool condition,
    std::string_view fail_msg,
    std::string_view success_msg = "",
    bool crash = false,
    const std::function<void()>& on_success = nullptr,
    const std::function<void()>& on_fail = nullptr
  );
}
