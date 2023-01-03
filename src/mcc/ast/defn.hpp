#ifndef MCC_AST_DEFN_HPP
#define MCC_AST_DEFN_HPP

#include "limits.hpp"
#include "scan/token.hpp"
#include "trait.hpp"
#include "type.hpp"
#include <array>

namespace mcc::ast {

enum class DefnKind {
  Var,
  Func,
  Enum,
  Struct,
  Primitive,
};

struct Defn {
  virtual ~Defn() {}
  virtual auto kind() const -> DefnKind = 0;
  Token name;
};

struct Var : Defn {
  auto kind() const -> DefnKind override {
    return DefnKind::Var;
  }
  Type type;
};

struct Func : Defn {
  auto kind() const -> DefnKind override {
    return DefnKind::Func;
  }

  Type type;
  std::array<Var *, max::FUNC_ARGS> args;
};

struct Enum : Defn {
  auto kind() const -> DefnKind override {
    return DefnKind::Enum;
  }

  std::array<Var *, max::ENUM_DECLS> decls;
};

struct Struct : Defn {
  auto kind() const -> DefnKind override {
    return DefnKind::Struct;
  }

  std::array<Var *, max::STRUCT_MEMBERS> members;
};

struct Primitive : Defn {
  auto kind() const -> DefnKind override {
    return DefnKind::Primitive;
  }

  enum Type : u32 {
    Auto   = KwAuto & TYPE_MASK,
    Double = KwDouble & TYPE_MASK,
    Char   = KwChar & TYPE_MASK,
    Float  = KwFloat & TYPE_MASK,
    Int    = KwInt & TYPE_MASK,
    Void   = KwVoid & TYPE_MASK,
  } type;
};

}  // namespace mcc::ast

#endif
