#include "cactus/application.hpp"
#include "cactus/http.hpp"
#include "cactus/utils.hpp"
#include "cactus/version.hpp"
#include "cactus/webhook.hpp"

#include <stdexcept>

#include <argparse/argparse.hpp>

inline static std::string api_version() {
  const std::string it =
      fmt::format("{}({})", cactus::GIT_VERSION, cactus::BUILD_TIME);
  return it;
}

void cactus::Application::http_server(const std::string &config_file,
                                      uint16_t port) const {
  spdlog::debug("load configuration from {}", config_file);
  if (!cactus::config_file_permission(config_file)) {
    throw std::invalid_argument(
        "invalid config file permissions, must be 400 or 600");
  }
  const toml::table config = toml::parse_file(config_file);

  httplib::Server server;

  auto gogs = config["gogs"].as_table();
  server.Post("/gogs/:id", [&](const httplib::Request &req,
                               httplib::Response &res) {
    {
      const auto client_id = req.path_params.at("id");
      spdlog::debug("call webhook gogs.{}", client_id);
      cactus::webhook::gogs::Handler handler(*gogs);
      handler.execute(client_id, req);
    }

    nlohmann::json body;
    {
      cactus::webhook::Ok ok;
      nlohmann::to_json(body, ok);
    }
    res.set_content(body.dump(), cactus::content_types::APPLICATION_JSON_UTF8);
  });
  server.Get("/version",
             [](const httplib::Request &req, httplib::Response &res) {
               const auto it = api_version();
               res.set_content(it, cactus::content_types::TEXT_PLAIN_UTF8);
             });

  server.set_logger(
      [](const httplib::Request &req, const httplib::Response &res) {
        spdlog::info("{} {} {}", req.method, req.path, res.status);
      });
  server.set_exception_handler(
      [](const auto &req, auto &res, std::exception_ptr ep) {
        try {
          std::rethrow_exception(ep);
        } catch (std::exception &e) {
          spdlog::error("{}", e.what());
          res.set_content(e.what(), cactus::content_types::TEXT_PLAIN_UTF8);
        } catch (...) {
          res.set_content("Unknown exception",
                          cactus::content_types::TEXT_PLAIN_UTF8);
        }
        res.status = httplib::StatusCode::InternalServerError_500;
      });

  spdlog::info("listen on http://0.0.0.0:{}", port);
  server.listen("0.0.0.0", port);
}

int cactus::Application::launch(int argc, char **argv) const {
  const std::string version = api_version();
  argparse::ArgumentParser program(cactus::PROJECT_NAME, version,
                                   argparse::default_arguments::help);

  program.add_description(cactus::PROJECT_DESCRIPTION);
  program.add_epilog(cactus::PROJECT_HOME);

  program.add_argument("-v", "--version").help("show version").flag();
  program.add_argument("-d", "--debug").help("run on debug mode").flag();

  argparse::ArgumentParser http_command("http");
  http_command.add_description("Start a HTTP server");
  http_command.add_argument("-c", "--config")
      .help("configuration file(toml)")
      .default_value("config.toml")
      .required();
  http_command.add_argument("-p", "--port")
      .help("port to listen on")
      .default_value(8080)
      .scan<'i', int>()
      .required();
  program.add_subparser(http_command);

  try {
    program.parse_args(argc, argv);
  } catch (const std::exception &err) {
    spdlog::error("{}", err.what());
    return EXIT_FAILURE;
  }

  if (program.get<bool>("--version") == true) {
    std::cout << version << std::endl;
    return EXIT_SUCCESS;
  }

  {
    spdlog::set_level(program.get<bool>("--debug") == true
                          ? spdlog::level::debug
                          : spdlog::level::info);
    spdlog::debug("run on debug mode");
  }

  if (program.is_subcommand_used(http_command)) {
    const std::string config = http_command.get<std::string>("--config");
    const int port = http_command.get<int>("--port");
    this->http_server(config, static_cast<uint16_t>(port));
    return EXIT_SUCCESS;
  }

  std::cout << program.help().str() << std::endl;
  return EXIT_SUCCESS;
}
