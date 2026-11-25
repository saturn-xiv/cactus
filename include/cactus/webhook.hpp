#pragma once

#include <httplib.h>
#include <toml++/toml.hpp>

#include "cactus/http.hpp"

namespace cactus {

namespace webhook {
struct Ok {
  Ok() : created_at(std::chrono::high_resolution_clock::now()) {}
  std::chrono::time_point<std::chrono::high_resolution_clock> created_at;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Ok, created_at)

class Handler {
 public:
  virtual void execute(const std::string& id, const httplib::Request& req) = 0;
};

// https://gogs.io/advancing/webhooks
namespace gogs {
struct Headers {
  std::string delivery;
  std::string event;
  std::string signature;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Headers, delivery, event, signature)

class Handler : public cactus::webhook::Handler {
 public:
  Handler(const toml::table& config) : _config(config) {}
  void execute(const std::string& id, const httplib::Request& req) override;

 private:
  toml::table _config;
};
}  // namespace gogs

// https://docs.github.com/en/webhooks/using-webhooks/creating-webhooks#creating-a-repository-webhook
// https://docs.github.com/en/webhooks/using-webhooks/validating-webhook-deliveries#validating-webhook-deliveries
// https://docs.github.com/en/webhooks/webhook-events-and-payloads#delivery-headers
namespace github {
struct Headers {
  std::string hook_id;
  std::string delivery;
  std::string event;
  std::string signature;
  std::string signature256;
  std::string user_agent;
  std::string installation_target_type;
  std::string installation_target_id;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Headers, hook_id, delivery, event, signature,
                                   signature256, user_agent,
                                   installation_target_type,
                                   installation_target_id)

class Handler : public cactus::webhook::Handler {
 public:
  Handler(const toml::table& config) : _config(config) {}
  void execute(const std::string& id, const httplib::Request& req) override;

 private:
  toml::table _config;
};
}  // namespace github

class Job {
 public:
  Job(const toml::table& config);
  template <typename T>
  std::pair<std::string, std::string> build(const T& headers,
                                            const std::string& body) const {
    nlohmann::json js = headers;
    return this->build(js.dump(), body);
  }
  std::pair<std::string, std::string> build(const nlohmann::json& headers,
                                            const std::string& body) const;

  inline std::string secret() const { return this->_secret; }

 private:
  std::string _id;
  std::string _secret;
  std::filesystem::path _script;
};
}  // namespace webhook

}  // namespace cactus
