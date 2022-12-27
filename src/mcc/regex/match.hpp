#ifndef MCC_REGEX_MATCH_HPP
#define MCC_REGEX_MATCH_HPP

#include "mcc.hpp"

namespace mcc::regex {

class Match {
public:
  Match(std::string_view expr, size_t index) : m_expr(expr), m_index(index) {}

  auto index() const -> size_t {
    return m_index;
  }

  auto begin() const -> const char * {
    return m_expr.begin();
  }

  auto end() const -> const char * {
    return m_index != npos() ? &m_expr[m_index] : m_expr.begin();
  }

  auto view() const -> std::string_view {
    return {begin(), end()};
  }

  auto next() const -> std::string_view {
    return {end(), m_expr.end()};
  }

  auto expr() const -> std::string_view {
    return m_expr;
  }

  operator bool() const {
    return m_index != npos();
  }

private:
  size_t m_index;
  std::string_view m_expr;
};

}  // namespace mcc::regex

#endif
