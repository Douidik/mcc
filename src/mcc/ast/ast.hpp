#ifndef MCC_AST_HPP
#define MCC_AST_HPP

#include "node.hpp"
#include <typeinfo>
#include <vector>

namespace mcc::ast {

constexpr auto none() -> u32 {
  return 0;
}

class Ast {
public:
  // Ast represents none-node at index 0
  Ast() : m_nodes(none()) {}

  auto push(auto node) -> u32 {
    m_nodes.push_back(node);
    return m_nodes.size() - 1;
  }

  template<typename T>
  auto at(u32 index) const -> const T & {
    return std::get<T>(m_nodes.at(index))();
  }

  template<typename T>
  auto at(u32 index) -> T & {
    return std::get<T>(m_nodes.at(index))();
  }

  auto begin() -> Node * {
    return &m_nodes[0];
  }

  auto end() -> Node * {
    return &m_nodes[m_nodes.size() - 1];
  }

private:
  std::vector<Node> m_nodes;
};

}  // namespace mcc::ast

#endif
