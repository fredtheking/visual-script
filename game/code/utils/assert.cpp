#include "utils/assert.hpp"
#include "core/log.hpp"
#include "utils/exceptions.hpp"

namespace vs::utils {
  bool sAssert(const bool condition, const string_view message, const bool crash) {
    if (condition) return true;

    if (crash) Log::crash(excs::sAssertExc{message});
    else       Log::error(message);
    return false;
  }

  bool cAssert(const bool condition, const string_view fail_msg, const string_view success_msg, const bool crash, cref<function<void()>> on_success, cref<function<void()>> on_fail) {
    if (!condition) {
      if (!fail_msg.empty()) {
        if (crash) Log::crash(excs::cAssertExc{fail_msg});
        else       Log::error(fail_msg);
      }
      if (on_fail) on_fail();
      return false;
    }

    if (on_success) on_success();
    if (!success_msg.empty()) Log::ok(success_msg);
    return true;
  }
}
