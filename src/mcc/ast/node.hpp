#ifndef MCC_AST_NODE_HPP
#define MCC_AST_NODE_HPP

#include "limits.hpp"
#include "scan/token.hpp"
#include <forward_list>
#include <variant>

namespace mcc::ast {

struct Stmt {};
struct Expr {};

struct Var {
  Token type;
  Token name;
};

struct Constant {
  Token value;
};

struct Func {
  Token type;
  Token name;
  Expr *args[max::FUNC_ARGS];
};

// <operation><expression>
struct UnaryExpr : Expr {
  Token op;
  Expr *expr;
};

// <expression[0]><operation><expression[1]>
struct BinaryExpr : Expr {
  Token op;
  Expr *expr[2];
};

// '[' <expression> ']'
struct IndexExpr : Expr {
  Token braces[2];
  Expr *expr;
};

// <function_name> '(' <expressions> ',' ... ')'
struct InvokeExpr : Expr {
  Token braces[2];
  Token commas[max::FUNC_ARGS - 2];
  Expr *args[max::FUNC_ARGS];
  Expr *func;
};

// <expression> <operation>
struct PostExpr : Expr {
  Token op;
  Expr *expr;
};

// <expression> '?' <expression> ':' <expression>
struct TernaryExpr : Expr {
  Token query;
  Token colon;
  Expr *expr[3];
};

// '(' <typename> ')' <expression>
struct CastExpr : Expr {
  Token braces[2];
  Token type;
  Expr *expr;
};

// '(' <expr> ')'
struct NestedExpr : Expr {
  Token braces[2];
  u32 depth;
  Expr *expr;
};

// '{' <stmt>, ... '}'
struct CompoundStmt : Stmt {
  Token braces[2];
  std::forward_list<Expr *> body;
};

// <kw> '(' <expr> ')' <head>
struct CondStmt : Stmt {
  Token kw;
  Token braces[2];
  Expr *expr;
  Stmt *head;
};

// <kw> '(' <expr> ')' <head>
struct LoopStmt : CondStmt {};

// <kw> <expr>?
struct JumpStmt : Stmt {
  Token kw;
  Expr *expr;
};

// Head definition:
// <type> <name> ('=' <expression>)? ';' | ','  <next_definition>
// Edges definition:
// <name> ('=' <expression>)? ','? <next_definition>
struct VarStmt : Stmt {
  struct Def {
    Var *var;
    Token assign;
    Expr *expr;
    Token comma;
  };

  Token type;
  std::forward_list<Def> defs;
};
  
using Node = std::variant<
  std::monostate,
  Var,
  Func,
  UnaryExpr,
  BinaryExpr,
  IndexExpr,
  InvokeExpr,
  PostExpr,
  TernaryExpr,
  CastExpr,
  NestedExpr,
  CompoundStmt,
  LoopStmt,
  JumpStmt,
  VarStmt>;

}  // namespace mcc::ast

#endif
