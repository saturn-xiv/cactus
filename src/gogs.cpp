#include "cactus/utils.hpp"
#include "cactus/webhook.hpp"

#include <cstdlib>
#include <format>
#include <stdexcept>

#include <spdlog/spdlog.h>

void cactus::webhook::gogs::Handler::execute(const std::string& id,
                                             const httplib::Request& req) {
  cactus::webhook::gogs::Headers headers{
      .delivery = req.get_header_value("X-Gogs-Delivery"),
      .event = req.get_header_value("X-Gogs-Event"),
      .signature = req.get_header_value("X-Gogs-Signature")};
  spdlog::info("receive gogs webhook {} {}", headers.event, headers.delivery);

  auto config = this->_config[id].as_table();
  if (config == nullptr) {
    throw std::invalid_argument("couldn't get configuration table for " + id);
  }
  const cactus::webhook::Job job(*config);
  if (cactus::hmac::sha256(job.secret(), req.body) != headers.signature) {
    throw std::invalid_argument("invalid signature");
  }
  const auto [jid, script] = job.build(headers, req.body);

  // {
  //   const std::optional<std::string> secret =
  //       (*config)["secret"].value<std::string>();

  // }

  // // TODO run template on background

  // const std::string uid = "gogs." + id + "." + headers.delivery;
  // {
  //   std::ofstream out(uid + ".json");
  //   out << req.body;
  //   out.close();
  // }

  // const std::optional<std::string> shell =
  //     (*config)["shell"].value<std::string>();

  // const std::string command =
  //     std::format("/bin/bash {0} {1}.json > {1}.out  2>&1", shell.value(),
  //     uid);
  // spdlog::info("execute {}", command);
  // int code = std::system(command.c_str());
  // if (code != EXIT_SUCCESS) {
  //   spdlog::error("{}", code);
  // }
}
