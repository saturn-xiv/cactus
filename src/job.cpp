#include "cactus/utils.hpp"
#include "cactus/webhook.hpp"

#include <cstdlib>
#include <format>
#include <stdexcept>

#include <spdlog/spdlog.h>
#include <cppcodec/base64_url_unpadded.hpp>
#include <inja/inja.hpp>

cactus::webhook::Job::Job(const toml::table& config) {
  this->_id = cactus::uuid();
  this->_secret = config["secret"].value<std::string>().value();
  this->_script = config["script"].value<std::string>().value();
}

std::pair<std::string, std::string> cactus::webhook::Job::build(
    const nlohmann::json& headers, const std::string& body) const {
  nlohmann::json data = {{"id", this->_id},
                         {"headers", headers},
                         {"payload", nlohmann::json::parse(body)}};
  inja::Environment env;
  const auto script = env.render_file(this->_script, data);
  return {this->_id, script};
}
