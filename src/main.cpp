#include "cactus/application.hpp"

#include <spdlog/spdlog.h>

int main(int argc, char **argv) {
  cactus::Application app;
  try {
    return app.launch(argc, argv);
  } catch (const std::exception &ex) {
    spdlog::error("{}", ex.what());
  } catch (...) {
    spdlog::error("an unknown exception occurred");
  }
  return EXIT_FAILURE;
}
