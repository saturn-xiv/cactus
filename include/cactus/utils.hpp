#pragma once

#include <string>

namespace cactus {
namespace hmac {
std::string sha256(const std::string &key, const std::string &data);
}
} // namespace cactus
