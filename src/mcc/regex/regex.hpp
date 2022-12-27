#ifndef MCC_REGEX_HPP
#define MCC_REGEX_HPP

#include "match.hpp"
#include "parser.hpp"
#include "stack.hpp"

namespace mcc::regex {

class Regex {
public:
  Regex(const char *src) : Regex{std::string_view(src)} {}

  Regex(std::string_view src) : m_src(src), m_stack() {
    m_head = Parser{src, m_stack}.parse();
  }

  auto head() const -> const Node * {
    return m_head;
  }

  auto stack() const -> std::span<const Node> {
    return m_stack.view();
  }

  auto src() const -> std::string_view {
    return m_src;
  }

  auto match(std::string_view expr) const -> Match {
    return m_head ? Match{expr, m_head->submit(expr, 0)} : Match{expr, npos()};
  }

  auto match(auto begin, auto end) const -> Match {
    return match(std::string_view{begin, end});
  }

private:
  const Node *m_head;
  Stack m_stack;
  std::string_view m_src;
};

}  // namespace mcc::regex

namespace mcc {
using regex::Regex;
}

namespace mcc::literals {
inline regex::Regex operator""_rx(const char *src, size_t) {
  return Regex{src};
}
}  // namespace mcc::literals

#endif
