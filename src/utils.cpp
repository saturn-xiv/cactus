#include "cactus/utils.hpp"

#include <iomanip>
#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/process.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/hmac.h>
#include <cryptopp/sha.h>

std::string cactus::uuid() {
  boost::uuids::uuid it = boost::uuids::random_generator()();
  return boost::uuids::to_string(it);
}

// https://cryptopp.com/wiki/HMAC
std::string cactus::hmac::sha256(const std::string& key,
                                 const std::string& data) {
  std::string mac, encoded;

  CryptoPP::HMAC<CryptoPP::SHA256> hmac(
      reinterpret_cast<const CryptoPP::byte*>(key.c_str()), key.size());
  CryptoPP::StringSource ssb(
      data, true,
      new CryptoPP::HashFilter(hmac, new CryptoPP::StringSink(mac)));
  CryptoPP::StringSource ssh(
      mac, true,
      new CryptoPP::HexEncoder(new CryptoPP::StringSink(encoded), false));

  return encoded;
}

bool cactus::config_file_permission(const std::filesystem::path& file) {
  const auto it = std::filesystem::status(file).permissions();

  return (it & std::filesystem::perms::owner_read) !=
             std::filesystem::perms::none &&
         (it & std::filesystem::perms::owner_exec) ==
             std::filesystem::perms::none &&
         (it & std::filesystem::perms::group_read) ==
             std::filesystem::perms::none &&
         (it & std::filesystem::perms::group_write) ==
             std::filesystem::perms::none &&
         (it & std::filesystem::perms::group_exec) ==
             std::filesystem::perms::none &&
         (it & std::filesystem::perms::others_read) ==
             std::filesystem::perms::none &&
         (it & std::filesystem::perms::others_write) ==
             std::filesystem::perms::none &&
         (it & std::filesystem::perms::others_exec) ==
             std::filesystem::perms::none;
}

std::expected<cactus::CommandResult, std::string> cactus::bash(
    const std::string& command) {
  try {
    const auto start = std::chrono::steady_clock::now();

    boost::asio::io_context ctx;
    boost::asio::readable_pipe out_pipe{ctx};
    boost::asio::readable_pipe err_pipe{ctx};

    boost::process::process proc(
        ctx, "/bin/sh", {"-c", command},
        boost::process::process_stdio{
            .in = nullptr, .out = out_pipe, .err = err_pipe});

    std::string out_str, err_str;
    boost::asio::async_read(out_pipe, boost::asio::dynamic_buffer(out_str),
                            [](auto, auto) {});
    boost::asio::async_read(err_pipe, boost::asio::dynamic_buffer(err_str),
                            [](auto, auto) {});

    ctx.run();
    int exit_code = proc.wait();
    const auto end = std::chrono::steady_clock::now();
    return cactus::CommandResult{
        .exit_code = exit_code,
        .stdout = std::move(out_str),
        .stderr = std::move(err_str),
        .elapsed =
            std::chrono::duration_cast<std::chrono::microseconds>(end - start)};
  } catch (const std::exception& e) {
    return std::unexpected(e.what());
  }
}
