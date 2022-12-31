#ifndef MCC_REGEX_NODE_HPP
#define MCC_REGEX_NODE_HPP

#include "mcc.hpp"
#include "state.hpp"
#include <compare>
#include <set>
#include <span>
#include <variant>

namespace mcc::regex {
class Node;

class Node {
  struct Cmp {
    auto operator()(const Node *lhs, const Node *rhs) const -> bool;
  };
  using Set = std::set<Node *, Cmp>;

public:
  Node() = default;
  Node(State state, size_t index);

  auto submit(std::string_view expr, size_t index) const -> size_t;
  auto end() -> Node *;
  auto concat(Node *node) -> Node *;
  auto map(u32 base) -> u32;
  auto members() -> Set;
  auto merge(Node *node) -> Node *;
  auto push(Node *node) -> Node *;
  auto insert(Node *node) -> Node *;

  auto index() const -> u32 {
    return m_index;
  }

  auto edges() -> Set & {
    return m_edges;
  }

  auto edges() const -> const Set & {
    return m_edges;
  }

  auto state() const -> const State & {
    return m_state;
  }

  auto breadth() const -> size_t {
    return m_edges.size();
  }

  auto max_edge() const -> Node * {
    return !m_edges.empty() ? *m_edges.rbegin() : nullptr;
  }

  auto branch() const -> bool {
    return !m_edges.empty() and max_edge()->index() > m_index;
  }

private:
  auto make_members(Set &set) -> Set &;

  State m_state;
  u32 m_index;
  Set m_edges;
};

}  // namespace mcc::regex

#endif
