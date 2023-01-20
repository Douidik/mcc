#ifndef MCC_WRITER_HPP
#define MCC_WRITER_HPP

#include "mcc.hpp"

namespace mcc {

class Writer {
public:
  auto write(std::string_view fmt, auto... args) {
    return fmt::format_to(m_buf, fmt, args...);
  }

  auto begin() const -> const char * {
    return m_buf.begin();
  }

  auto end() const -> const char * {
    return m_buf.end();
  }

private:
  fmt::memory_buffer m_buf;
};

}  // namespace mcc

#endif
