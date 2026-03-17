#pragma once

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif
#include <raylib.h>
#include <raymath.h>

// INCLUDES
#include <iostream>
#include <string>
#include <cmath>
#include <optional>
#include <expected>
#include <memory>
#include <queue>
#include <any>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <chrono>

#include "utils/assert.hpp"

// ALIASES
using
std::vector,
std::initializer_list,
std::unordered_map,
std::string,
std::string_view,
std::to_string,
std::ostringstream,
std::optional,
std::nullopt,
std::make_unique,
std::make_shared,
std::function,
std::any;

using
std::chrono::high_resolution_clock,
std::chrono::time_point;

template <typename T> using smart_ptr_object = std::enable_shared_from_this<T>;

using
vs::utils::sAssert,
vs::utils::cAssert;

template <typename T> using cref = const T&;
template <typename T> using maybe = cref<optional<T>>;
template <typename T> using optional_ref = optional<std::reference_wrapper<T>>;

using uchar = uint8_t;
using ushort = uint16_t;
using uint = uint32_t;
using ulong = uint64_t;
using byte = unsigned char;

template <typename T, typename D = std::default_delete<T>> using up = std::unique_ptr<T, D>;
template <typename T> using sp = std::shared_ptr<T>;
template <typename T> using wp = std::weak_ptr<T>;


#define FABRIC_CREATE(NAME, ...)                                        \
  /* The static method that fully initializes itself and its children */ \
  static sp<NAME> $create(__VA_ARGS__);

#define STRUCTDIR(NAME, PARENT, PARENT_VAR_NAME, ...)                                        \
struct NAME##_t {                                                                             \
  __VA_ARGS__                                                                                  \
public:                                                                                         \
  explicit NAME##_t(wp<PARENT> PARENT_VAR_NAME): PARENT_VAR_NAME(std::move(PARENT_VAR_NAME)) {}  \
private:                                                                                          \
  wp<PARENT> PARENT_VAR_NAME;                                                                      \
};                                                                                                  \
up<NAME##_t> NAME = nullptr;                                                                         \
friend struct NAME##_t;

#define STATIC_ONLY_CONSTRUCTOR(NAME) \
  public:                              \
    NAME() = delete;                    \
    NAME(cref<NAME>) = delete;           \
    NAME& operator=(cref<NAME>) = delete;

#if defined(_MSC_VER)
#define DEBUG_BREAK __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
#include <csignal>
#define DEBUG_BREAK raise(SIGTRAP)
#else
#define DEBUG_BREAK ((void)0)
#endif