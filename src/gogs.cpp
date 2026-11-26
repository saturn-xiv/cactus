#include "cactus/utils.hpp"
#include "cactus/webhook.hpp"

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
  const auto task = job.build(headers, req.body);
  this->_queue->push(task);
}
