#ifndef MCC_AST_HPP
#define MCC_AST_HPP

#include "defn.hpp"
#include "node.hpp"
#include "trait.hpp"
#include <forward_list>
#include <unordered_map>
#include <utility>

namespace mcc::ast {

class Ast {
public:
  Ast() : m_nodes() {}

  ~Ast() {
    for (Node *node : m_nodes) {
      delete (node);
    }
    for (auto [_, object] : m_defn_map) {
      delete (object);
    }
    m_nodes = {};
  }

  template<typename T>
  auto push(T *node) -> T * {
    m_nodes.emplace_front(static_cast<Node *>(node));
    return node;
  }

  template<typename T>
  auto define(T *object) -> T * {
    m_defn_map[object->name.view] = static_cast<Defn *>(object);
    return object;
  }

  auto nodes() const -> const std::forward_list<Node *> & {
    return m_nodes;
  }

  auto resolve(Token name) const -> Defn * {
    if (auto it = m_defn_map.find(name.view); it != m_defn_map.end()) {
      return it->second;
    }
    return {};
  }

private:
  std::forward_list<Node *> m_nodes;
  std::unordered_map<std::string_view, Defn *> m_defn_map;
};

}  // namespace mcc::ast

namespace mcc {
using Ast = ast::Ast;
}

#endif
