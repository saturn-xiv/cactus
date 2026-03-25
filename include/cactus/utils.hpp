#pragma once

#include <filesystem>
#include <string>

namespace cactus {
bool config_file_permission(const std::filesystem::path &file);
namespace hmac {
std::string sha256(const std::string &key, const std::string &data);
}
} // namespace cactus
