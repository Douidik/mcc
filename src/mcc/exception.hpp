#ifndef MCC_EXCEPTION_HPP
#define MCC_EXCEPTION_HPP

#include <exception>
#include <fmt/format.h>
#include <string>
#include <string_view>

namespace mcc {

class Exception : public std::exception {
public:
  Exception(std::string_view name, std::string_view fmt, auto... args) :
    m_buffer(fmt::format(fmt::runtime(fmt), args...)) {}

  auto what() const noexcept -> const char * override {
    return m_buffer.data();
  }

  auto name() const noexcept -> std::string_view {
    return m_name;
  }

private:
  std::string_view m_name;
  std::string m_buffer;
};

}  // namespace mcc

#endif
