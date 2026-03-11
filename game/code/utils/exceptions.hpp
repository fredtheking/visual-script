#pragma once
#include "pch.hpp"
#include "utils/string_manip.hpp"

#define NEW_RUNTIME_EXCEPTION(NAME)                                   \
struct NAME##Exc final : std::runtime_error {                          \
  template<typename... Args>                                            \
  explicit NAME##Exc(FORMAT_ARGS)                                        \
    : std::runtime_error(utils::format(fmt, std::forward<Args>(args)...)) \
  {}                                                                       \
};

namespace vs::excs {
  NEW_RUNTIME_EXCEPTION(sAssert)
  NEW_RUNTIME_EXCEPTION(cAssert)

  NEW_RUNTIME_EXCEPTION(Shader_NotFound)
}
