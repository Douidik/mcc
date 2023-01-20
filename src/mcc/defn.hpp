#ifndef MCC_DEFN_HPP
#define MCC_DEFN_HPP

#include "limits.hpp"
#include "scan/token.hpp"
#include "type.hpp"
#include <array>
#include <string_view>

namespace mcc {

enum class DefnKind { Var, Func, Struct, EnumConstant, StringConstant, Primitive };

class Defn {
public:
  Defn(std::string_view name) : m_name(name) {}
  virtual ~Defn() = default;

  virtual auto kind() const -> DefnKind = 0;

  auto name() const -> std::string_view {
    return m_name;
  }

private:
  std::string_view m_name;
};

class Var : public Defn {
public:
  Var(Type type, std::string_view name) : Defn(name), m_type(type) {}

  auto kind() const -> DefnKind override {
    return DefnKind::Var;
  }

private:
  Type m_type;
};

class Func : public Defn {
public:
  Func(Type type, std::string_view name, auto params) :
    Defn(name),
    m_type(type),
    m_params(params) {}

  auto kind() const -> DefnKind override {
    return DefnKind::Func;
  }

private:
  Type m_type;
  std::array<Var *, max::func_args> m_params;
};

class Struct : public Defn {
private:
  Struct(std::string_view name, auto members) : Defn(name), m_members(members) {}

  auto kind() const -> DefnKind override {
    return DefnKind::Struct;
  }

private:
  std::array<Var *, max::struct_members> m_members;
};

class EnumConstant : public Defn {
public:
  EnumConstant(std::string_view name, Type type, Token value) :
    Defn(name),
    m_type(type),
    m_value(value) {}

  auto kind() const -> DefnKind override {
    return DefnKind::EnumConstant;
  }

private:
  Type m_type;
  Token m_value;
};

// NOTE: we lookup string-constants by value
class StringConstant : public Defn {
public:
  StringConstant(Token string) : Defn(string.src) {}

  auto hash() const -> u64 {
    return std::hash<std::string_view>{}(name());
  }

  auto kind() const -> DefnKind override {
    return DefnKind::StringConstant;
  }
};

class Primitive : public Defn {
public:
  Primitive(std::string_view name, size_t size) : Defn(name), m_size(size) {}

  auto kind() const -> DefnKind override {
    return DefnKind::Primitive;
  }

  static auto defn_void() -> Primitive {
    return {"void", 0};
  }
  static auto defn_char() -> Primitive {
    return {"char", 1};
  }
  static auto defn_short() -> Primitive {
    return {"short", 2};
  }
  static auto defn_int() -> Primitive {
    return {"int", 4};
  }
  static auto defn_long() -> Primitive {
    return {"long", 8};
  }
  static auto defn_float() -> Primitive {
    return {"float", 4};
  }
  static auto defn_double() -> Primitive {
    return {"double", 8};
  }
  static auto defn_signed() -> Primitive {
    return {"signed", 4};
  }
  static auto defn_unsigned() -> Primitive {
    return {"unsigned", 4};
  }

private:
  size_t m_size;
};

}  // namespace mcc

#endif
