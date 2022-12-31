#ifndef MCC_AST_HPP
#define MCC_AST_HPP

#include "node.hpp"
#include <forward_list>

namespace mcc::ast {

class Ast {
  using List = std::forward_list<Node>;

public:
  Ast() : m_nodes() {}

  template<typename T, typename... Ts>
  auto push(Ts &&...args) -> T & {
    return m_nodes.emplace_front(T{args...});
  }

  auto nodes() const -> const List & {
    return m_nodes;
  }

private:
  List m_nodes;
};

}  // namespace mcc::ast

#endif
