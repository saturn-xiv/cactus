#pragma once

#include <chrono>
#include <expected>
#include <filesystem>
#include <functional>
#include <print>
#include <string>
#include <utility>

namespace cactus {
bool config_file_permission(const std::filesystem::path& file);
namespace hmac {
std::string sha256(const std::string& key, const std::string& data);
}
std::string uuid();

struct CommandResult {
  inline bool succeed() const { return this->exit_code == 0; }
  int exit_code;
  std::string stdout;
  std::string stderr;
  std::chrono::microseconds elapsed;
};
std::expected<CommandResult, std::string> bash(const std::string& command);
}  // namespace cactus
