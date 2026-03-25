#include "cactus/utils.hpp"
#include "cactus/webhook.hpp"

#include <cstdlib>
#include <format>
#include <stdexcept>

#include <cppcodec/base64_url_unpadded.hpp>
#include <spdlog/spdlog.h>

void cactus::webhook::gogs::Handler::execute(const std::string &id,
                                             const httplib::Request &req) {

  const auto delivery = req.get_header_value("X-Gogs-Delivery");
  const auto event = req.get_header_value("X-Gogs-Event");
  spdlog::info("{} {}", event, delivery);

  auto config = this->_config[id].as_table();
  if (config == nullptr) {
    throw std::invalid_argument("couldn't get configuration table for " + id);
  }

  const std::optional<std::string> secret =
      (*config)["secret"].value<std::string>();

  {
    const auto signature = req.get_header_value("X-Gogs-Signature");
    if (cactus::hmac::sha256(secret.value(), req.body) != signature) {
      throw std::invalid_argument("invalid signature");
    }
  }

  const std::string uid = "gogs." + id + "." + delivery + ".json";
  {
    std::ofstream out(uid + ".json");
    out << req.body;
    out.close();
  }

  const std::optional<std::string> shell =
      (*config)["shell"].value<std::string>();

  const std::string command =
      std::format("/bin/bash {0} {1}.json > {1}.out  2>&1", shell.value(), uid);
  spdlog::info("execute {}", command);
  int code = std::system(command.c_str());
  if (code != EXIT_SUCCESS) {
    spdlog::error("{}", code);
  }
}
