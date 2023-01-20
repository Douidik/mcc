#ifndef MCC_STMT_HPP
#define MCC_STMT_HPP

#include "node.hpp"
#include "scan/token.hpp"
#include <optional>
#include <vector>

namespace mcc {

class Stmt : public Node {};

class MainStmt : public Stmt {
  auto graph(GraphContext &ctx) -> GraphContext & override;
  auto asm_x86(AsmContext &ctx) -> AsmContext & override;
};

class CompoundStmt : public Stmt {
public:
  CompoundStmt(Token open, auto body, Token close) : m_braces{open, close}, m_body(body) {}

private:
  Token m_braces[2];
  std::vector<Stmt *> m_body;
};

class CondStmt : public Stmt {
public:
  CondStmt(Token keyword, struct NestedExpr *expr, CompoundStmt *body) :
    m_keyword(keyword),
    m_expr(expr),
    m_body(body) {}

private:
  Token m_keyword;
  struct NestedExpr *m_expr;
  CompoundStmt *m_body;
};

class LoopStmt : public Stmt {
public:
  LoopStmt(Token keyword, struct NestedExpr *expr, CompoundStmt *body) :
    m_keyword(keyword),
    m_expr(expr),
    m_body(body) {}

private:
  Token m_keyword;
  struct NestedExpr *m_expr;
  CompoundStmt *m_body;
};

class InitStmt : public Stmt {
public:
  InitStmt(struct Var *var, struct Expr *expr) : m_var(var), m_expr(expr) {}

private:
  struct Var *m_var;
  struct Expr *m_expr;
};

class FuncStmt : public Stmt {
public:
  FuncStmt(struct Func *func, CompoundStmt *body) : m_func(func), m_body(body) {}

private:
  struct Func *m_func;
  CompoundStmt *m_body;
};

class StructStmt : public Stmt {
  StructStmt(Token keyword, struct Struct *structure) : m_keyword(keyword), m_struct(structure) {}

private:
  Token m_keyword;
  struct Struct *m_struct;
};

}  // namespace mcc

#endif
