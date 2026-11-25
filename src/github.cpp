#include "cactus/utils.hpp"
#include "cactus/webhook.hpp"

#include <cstdlib>
#include <format>
#include <stdexcept>

#include <spdlog/spdlog.h>

void cactus::webhook::github::Handler::execute(const std::string& id,
                                               const httplib::Request& req) {
  cactus::webhook::github::Headers headers{
      .hook_id = req.get_header_value("X-GitHub-Hook-ID"),
      .delivery = req.get_header_value("X-GitHub-Delivery"),
      .event = req.get_header_value("X-GitHub-Event"),
      .signature = req.get_header_value("X-Hub-Signature"),
      .signature256 = req.get_header_value("X-Hub-Signature-256"),
      .user_agent = req.get_header_value("User-Agent"),
      .installation_target_type =
          req.get_header_value("X-GitHub-Hook-Installation-Target-Type"),
      .installation_target_id =
          req.get_header_value("X-GitHub-Hook-Installation-Target-ID")};
  spdlog::info("receive github webhook {} {} {}", headers.hook_id,
               headers.event, headers.delivery);

  auto config = this->_config[id].as_table();
  if (config == nullptr) {
    throw std::invalid_argument("couldn't get configuration table for " + id);
  }
  const cactus::webhook::Job job(*config);
  if (cactus::hmac::sha256(job.secret(), req.body) != headers.signature256) {
    throw std::invalid_argument("invalid signature");
  }
  const auto [jid, script] = job.build(headers, req.body);

  // const std::optional<std::string> secret =
  //     (*config)["secret"].value<std::string>();

  // {
  //   if (cactus::hmac::sha256(secret.value(), req.body) !=
  //       headers.signature256) {
  //     throw std::invalid_argument("invalid signature");
  //   }
  // }

  // const std::string uid = "github." + id + "." + headers.delivery;
  // {
  //   std::ofstream out(uid + ".json");
  //   out << req.body;
  //   out.close();
  // }

  // const std::optional<std::string> shell =
  //     (*config)["shell"].value<std::string>();

  // // TODO run on background
  // const std::string command =
  //     std::format("/bin/bash {0} {1}.json > {1}.out  2>&1", shell.value(),
  //     uid);
  // spdlog::info("execute {}", command);
  // int code = std::system(command.c_str());
  // if (code != EXIT_SUCCESS) {
  //   spdlog::error("{}", code);
  // }
}
