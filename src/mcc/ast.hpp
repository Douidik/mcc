#ifndef MCC_AST_HPP
#define MCC_AST_HPP

#include "defn.hpp"
#include "node.hpp"
#include <unordered_map>
#include <vector>

namespace mcc {

class Ast {
public:
  ~Ast() {
    for (auto node : m_nodes)
      delete node;
    for (auto defn : m_defn_map)
      delete defn.second;
  }

  template<typename T>
  auto push(T *node) -> T * {
    return m_nodes.emplace_back(static_cast<Node *>(node));
  }

  template<typename T>
  auto defn(T *defn) -> T * {
    return m_defn_map.try_emplace(defn->name(), defn).second;
  }

  auto find(std::string_view name) const -> Defn * {
    auto it = m_defn_map.find(name);
    return it != m_defn_map.end() ? it->second : nullptr;
  }

private:
  std::vector<Node *> m_nodes;
  std::unordered_map<std::string_view, Defn *> m_defn_map;
};

}  // namespace mcc

#endif
