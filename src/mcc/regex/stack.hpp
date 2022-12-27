#ifndef MCC_REGEX_STACK_HPP
#define MCC_REGEX_STACK_HPP

#include "node.hpp"

namespace mcc::regex {
constexpr size_t STACK_CAPACITY = 128;

class Stack {
public:
  auto push(State state, size_t index = 0) -> Node * {
    if (m_size + 1 > STACK_CAPACITY) {
      throw Exception("regex exception", "stack size exceeded capacity");
    }
    return &(m_stack.at(m_size++) = Node{state, index});
  }

  auto empty() const -> bool {
    return !m_size;
  }

  auto view() -> std::span<Node> {
    return {m_stack.data(), m_size};
  }

  auto view() const -> std::span<const Node> {
    return {m_stack.data(), m_size};
  }

private:
  size_t m_size;
  std::array<Node, STACK_CAPACITY> m_stack;
};

}  // namespace mcc::regex

#endif
