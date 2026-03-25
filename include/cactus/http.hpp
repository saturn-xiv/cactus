#pragma once

#include <chrono>
#include <format>
#include <string>

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

namespace cactus {
namespace content_types {
inline static const std::string TEXT_PLAIN_UTF8 = "text/plain; charset=utf-8";
inline static const std::string APPLICATION_JSON_UTF8 =
    "application/json; charset=utf-8";
} // namespace content_types
} // namespace cactus

namespace nlohmann {
template <typename Clock, typename Duration>
struct adl_serializer<std::chrono::time_point<Clock, Duration>> {
  static void to_json(nlohmann::json &j,
                      const std::chrono::time_point<Clock, Duration> &o) {
    j = std::format("{:%FT%T%z}", o);
  }

  static void from_json(const nlohmann::json &j,
                        std::chrono::time_point<Clock, Duration> &o) {
    const std::string s = j.get<std::string>();
    std::istringstream in{s};
    in >> std::chrono::parse("%FT%T%z", o);
    if (in.fail()) {
      spdlog::error("failed to parse {}", s);
    }
  }
};
} // namespace nlohmann
