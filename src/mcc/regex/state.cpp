#include "state.hpp"
#include "node.hpp"

namespace mcc::regex {

auto State::submit(std::string_view expr, size_t index) const -> size_t {
  Option option = static_cast<Option>(m_variant.index());

  if (option != Option::Epsilon && index >= expr.size()) {
    return npos();
  }

  switch (option) {
  case Option::Epsilon: return index;
  case Option::Any: return index + 1;
  case Option::None: return npos();

  case Option::Dash: {
    auto match = std::get<Dash>(m_variant).sequence->submit(expr, index);
    return match != npos() ? index : npos();
  }

  case Option::Not: {
    auto match = std::get<Not>(m_variant).sequence->submit(expr, index);
    return match != npos() ? npos() : index + 1;
  }

  case Option::Text: {
    auto content = std::get<Text>(m_variant).content;
    auto compared = expr.substr(index, content.size());
    return compared != content ? npos() : index + content.size();
  }

  case Option::Set: {
    return std::get<Set>(m_variant).content.find(expr[index]) != npos() ? index + 1 : npos();
  }

  case Option::Range: {
    auto [a, b] = std::get<Range>(m_variant);
    return a <= expr[index] and expr[index] <= b ? index + 1 : npos();
  }

  default: return npos();
  }
}

auto State::option() const -> Option {
  return static_cast<Option>(m_variant.index());
}

auto State::has(Option option) const -> bool {
  return static_cast<Option>(m_variant.index()) == option;
}

}  // namespace mcc::regex
