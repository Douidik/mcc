#ifndef MCC_REGEX_STATE_HPP
#define MCC_REGEX_STATE_HPP

#include "mcc.hpp"
#include <variant>

namespace mcc::regex {
class Node;

struct Epsilon {};
struct Any {};
struct None {};
struct Not {
  Node *sequence;
};
struct Dash {
  Node *sequence;
};
struct Text {
  std::string_view content;
};
struct Set {
  std::string_view content;
};
struct Range {
  char a, b;
};

enum class Option : u32 {
  Epsilon,
  Any,
  None,
  Not,
  Dash,
  Text,
  Set,
  Range,
};

using Variant = std::variant<Epsilon, Any, None, Not, Dash, Text, Set, Range>;

struct State {
public:
  State() = default;
  State(auto state) : m_variant(state) {}

  auto variant() const -> const Variant & {
    return m_variant;
  }

  auto submit(std::string_view expr, size_t index) const -> size_t;
  auto size() const -> size_t;
  auto option() const -> Option;
  auto has(Option option) const -> bool;

private:
  Variant m_variant;
};

}  // namespace mcc::regex

#endif
