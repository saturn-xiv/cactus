#include "cactus/webhook.hpp"
#include "cactus/utils.hpp"

#include <cstdlib>
#include <format>
#include <stdexcept>

#include <inja/inja.hpp>

void cactus::webhook::consume(const std::filesystem::path& working_dir,
                              const std::string& id, const cactus::Email& email,
                              const std::string& script) {
  spdlog::debug("{} consume {}", __PRETTY_FUNCTION__, id);
  const auto root = working_dir / id;
  if (!std::filesystem::create_directories(root)) {
    spdlog::error("failed to create working dir {}", root.string());
    return;
  }

  const auto run_sh = root / "run.sh";
  spdlog::debug("generate file {}", run_sh.string());
  {
    std::ofstream out(run_sh);
    out << script;
    out.close();
  }

  const std::string command = std::format("/usr/bin/bash {}", run_sh.string());
  const auto res = cactus::bash(command);
  if (!res) {
    spdlog::error("{} {} {}", __PRETTY_FUNCTION__, id, res.error());
    return;
  }
  const std::string elapsed = std::format("{}", res->elapsed);
  if (res->succeed()) {
    spdlog::debug("{} {}", res->exit_code, res->stdout);
  } else {
    spdlog::error("{} {}\n{}", res->exit_code, res->stdout, res->stderr);
  }

  nlohmann::json data = {{"id", id},
                         {"exit_code", res->exit_code},
                         {"stdout", res->stdout},
                         {"stderr", res->stderr},
                         {"elapsed", elapsed}};
  const auto subject = inja::render("Execute {{ id }} in {{ elapsed }}", data);
  const auto body = inja::render(R"TPL(
{{ exit_code }} {{ elapsed }}
-----------------------------------------------------------
{{ stdout }}
-----------------------------------------------------------
{{ stderr }}
-----------------------------------------------------------
)TPL",
                                 data);

  email.send(subject, body);
  spdlog::debug("{} done({})", __PRETTY_FUNCTION__, id);
}
