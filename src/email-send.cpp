#include "cactus/worker.hpp"

#include <spdlog/spdlog.h>
#include <mailio/message.hpp>
#include <mailio/smtp.hpp>

cactus::Email::Email(const toml::table& config) {
  this->_host = config["host"].value<std::string>().value();
  this->_port = config["port"].value<uint16_t>().value_or(465);
  this->_user = config["user"].value<std::string>().value();
  this->_password = config["password"].value<std::string>().value();
  if (const toml::array* it = config["cc"].as_array()) {
    for (const toml::node& el : *it) {
      if (auto s = el.value<std::string>()) {
        this->_cc.push_back(*s);
      }
    }
  }
  if (const toml::array* it = config["bcc"].as_array()) {
    for (const toml::node& el : *it) {
      if (auto s = el.value<std::string>()) {
        this->_bcc.push_back(*s);
      }
    }
  }
}

void cactus::Email::send(const std::string& subject,
                         const std::string& body) const {
  spdlog::info("send email '{}'", subject);

  mailio::message msg;
  msg.from(mailio::mail_address(
      mailio::string_t("", "UTF-8", mailio::codec::codec_t::BASE64),
      this->_user));
  for (const auto& it : this->_cc) {
    msg.add_cc_recipient(mailio::mail_address("", it));
  }
  for (const auto& it : this->_bcc) {
    msg.add_bcc_recipient(mailio::mail_address("", it));
  }

  msg.subject(subject, mailio::codec::codec_t::BASE64);
  msg.content_type(mailio::message::media_type_t::TEXT, "plain", "utf-8");
  msg.content_transfer_encoding(
      mailio::mime::content_transfer_encoding_t::BASE_64);
  msg.content(body);

  mailio::smtps con(this->_host, this->_port);
  con.authenticate(this->_user, this->_password,
                   mailio::smtps::auth_method_t::LOGIN);

  con.submit(msg);
}
