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
  virtual void execute(const std::string &id, const httplib::Request &req) = 0;
};

namespace gogs {
class Handler : public cactus::webhook::Handler {
public:
  Handler(const toml::table &config) : _config(config) {}
  void execute(const std::string &id, const httplib::Request &req) override;

private:
  toml::table _config;
};
} // namespace gogs

} // namespace webhook

} // namespace cactus
