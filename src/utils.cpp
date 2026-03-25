#include "cactus/utils.hpp"

#include <iomanip>
#include <iostream>
#include <string>

#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/hmac.h>
#include <cryptopp/sha.h>

// https://cryptopp.com/wiki/HMAC
std::string cactus::hmac::sha256(const std::string &key,
                                 const std::string &data) {
  std::string mac, encoded;

  CryptoPP::HMAC<CryptoPP::SHA256> hmac(
      reinterpret_cast<const CryptoPP::byte *>(key.c_str()), key.size());
  CryptoPP::StringSource ssb(
      data, true,
      new CryptoPP::HashFilter(hmac, new CryptoPP::StringSink(mac)));
  CryptoPP::StringSource ssh(
      mac, true,
      new CryptoPP::HexEncoder(new CryptoPP::StringSink(encoded), false));

  return encoded;
}
