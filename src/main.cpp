#include "cactus/application.hpp"

#include <boost/exception/diagnostic_information.hpp>

#include <spdlog/spdlog.h>

int main(int argc, char** argv) {
  cactus::Application app;
  try {
    return app.launch(argc, argv);
  } catch (const std::exception& ex) {
    spdlog::error("{}", ex.what());
  } catch (...) {
    spdlog::error(boost::current_exception_diagnostic_information());
  }
  return EXIT_FAILURE;
}
