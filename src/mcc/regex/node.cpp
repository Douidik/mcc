#include "node.hpp"
#include <algorithm>

namespace mcc::regex {

auto Node::Cmp::operator()(const Node *lhs, const Node *rhs) const -> bool {
  return lhs->index() < rhs->index();
}

Node::Node(State state, size_t index) : m_edges(), m_index(index), m_state(state) {}

auto Node::submit(std::string_view expr, size_t index) const -> size_t {
  auto match = m_state.submit(expr, index);

  if (match != npos()) {
    if (ok() and match >= expr.size()) return match;

    for (const Node *edge : m_edges) {
      auto match_fwd = edge->submit(expr, match);
      if (match_fwd != npos()) return match_fwd;
    }

    if (ok()) return match;
  }

  return npos();
}

auto Node::end() const -> const Node * {
  const Node *end = this;

  for (const Node *edge : m_edges) {
    if (edge->index() > index()) end = &std::max(*end, *edge->end());
  }

  return end;
}

auto Node::members() -> Set {
  Set set{this};

  for (Node *edge : m_edges) {
    if (edge->index() > index()) set.merge(edge->members());
  }

  return set;
}

auto Node::concat(Node *node) -> Node * {
  for (auto member : members()) {
    if (!member->branch()) member->insert(node);
  }

  return node;
}

auto Node::map(u32 base) -> u32 {
  for (auto member : members()) {
    member->m_index += base;
  }

  return base;
}

auto Node::merge(Node *node) -> Node * {
  node->map(end()->index() + 1);
  return concat(node);
}

auto Node::push(Node *node) -> Node * {
  node->map(end()->index() + 1);
  return insert(node);
}

auto Node::insert(Node *node) -> Node * {
  return *m_edges.insert(node).first;
}

}  // namespace mcc::regex
