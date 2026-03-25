#pragma once

#include <cstdint>
#include <string>

namespace cactus {
class Application {
public:
  Application(){};
  int launch(int argc, char **argv) const;

private:
  void http_server(const std::string &config_file, uint16_t port) const;
};
} // namespace cactus
