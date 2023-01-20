#ifndef MCC_EXPR_HPP
#define MCC_EXPR_HPP

#include "limits.hpp"
#include "node.hpp"
#include "scan/token.hpp"
#include "type.hpp"
#include <array>
#include <span>

namespace mcc {

enum class Order {
  Prev,
  Post,
};

class Expr : public Node {
public:
private:
};

class IdExpr : public Expr {
public:
  IdExpr(struct Defn *defn) : m_defn(defn) {}

private:
  struct Defn *m_defn;
};

class ConstantExpr : public Expr {
public:
  ConstantExpr(Token constant) : m_constant(constant) {}

private:
  Token m_constant;
};

class UnaryExpr : public Expr {
public:
  UnaryExpr(Order order, Expr *expr, Token op) : m_order(order), m_expr(expr), m_op(op) {}

  auto expr() const -> Expr * {
    return m_expr;
  }

private:
  Order m_order;
  Expr *m_expr;
  Token m_op;
};

class BinaryExpr : public Expr {
public:
  BinaryExpr(Expr *lhs, Expr *rhs, Token op) : m_lhs(lhs), m_rhs(rhs), m_op(op) {}

  auto lhs() const -> Expr * {
    return m_lhs;
  }
  auto rhs() const -> Expr * {
    return m_rhs;
  }

private:
  Expr *m_lhs;
  Expr *m_rhs;
  Token m_op;
};

class IndexExpr : public Expr {
public:
  IndexExpr(Expr *expr) : m_expr(expr) {}

  auto expr() const -> Expr * {
    return m_expr;
  }

private:
  Expr *m_expr;
};

class InvokeExpr : public Expr {
public:
  InvokeExpr(struct Func *func, auto args) : m_func(func), m_args(args) {}

private:
  struct Func *m_func;
  std::array<Expr *, max::func_args> m_args;
};

class TernaryExpr : public Expr {
public:
  TernaryExpr(Expr *cond, Expr *lhs, Expr *rhs) : m_cond(cond), m_lhs(lhs), m_rhs(rhs) {}

private:
  Expr *m_cond;
  Expr *m_lhs;
  Expr *m_rhs;
};

class CastExpr : public Expr {
public:
  CastExpr(Type type, Expr *expr) : m_type(type), m_expr(expr) {}

private:
  Type m_type;
  Expr *m_expr;
};

class NestedExpr : public Expr {
public:
  NestedExpr(Expr *expr) : m_expr(expr) {}

  auto expr() const -> Expr * {
    return m_expr;
  }

private:
  Expr *m_expr;
};

}  // namespace mcc

#endif
