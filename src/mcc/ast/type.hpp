#ifndef MCC_AST_TYPE_HPP
#define MCC_AST_TYPE_HPP

#include "scan/token.hpp"
#include <variant>

namespace mcc::ast {

struct Type {
  Token name;
  Token mods;
  struct Defn *defn;
};

}  // namespace mcc::ast

#endif
