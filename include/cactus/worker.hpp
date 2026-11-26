#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <optional>
#include <queue>
#include <stop_token>
#include <thread>
#include <vector>

#include <toml++/toml.hpp>

namespace cactus {
namespace worker {
template <typename T>
class Queue {
 public:
  void push(T message) {
    {
      std::unique_lock<std::mutex> lock(this->_mutex);
      this->_queue.push(std::move(message));
    }
    this->_variable.notify_one();
  }

  std::optional<T> pop(std::stop_token stoken) {
    std::unique_lock<std::mutex> lock(this->_mutex);
    if (!this->_variable.wait(lock, stoken,
                              [this] { return !_queue.empty(); })) {
      return std::nullopt;
    }

    T value = std::move(this->_queue.front());
    this->_queue.pop();
    return value;
  }

 private:
  std::queue<T> _queue;
  std::mutex _mutex;
  std::condition_variable_any _variable;
};

class Email {
 public:
  Email(const toml::table& config);
  void send(const std::string& to, const std::vector<std::string>& cc,
            const std::vector<std::string>& bcc, const std::string& subject,
            const std::string& body) const;

 private:
};
}  // namespace worker
}  // namespace cactus
