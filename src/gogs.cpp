#include "cactus/webhook.hpp"

#include <stdexcept>

#include <spdlog/spdlog.h>

void cactus::webhook::gogs::Handler::execute(const std::string &id,
                                             const httplib::Request &req) {

  std::optional<std::string_view> secret =
      this->_config[id].value<std::string_view>();
  if (!secret) {
    throw std::invalid_argument("couldn't get the secret key");
  }
  // TODO
}
