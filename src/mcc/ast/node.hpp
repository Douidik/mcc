#ifndef MCC_AST_NODE_HPP
#define MCC_AST_NODE_HPP

#include "limits.hpp"
#include "scan/token.hpp"
#include <variant>

namespace mcc::ast {

struct Stmt {};
struct Expr {};

struct Var {
  Token type;
  Token name;
};

struct Func {
  Token type;
  Token name;
  u32 args[max::FUNC_ARGS];
};

// <operation><expression>
struct UnaryExpr : Expr {
  Token op;
  u32 expr;
};

// <expression[0]><operation><expression[1]>
struct BinaryExpr : Expr {
  Token op;
  u32 expr[2];
};

// '[' <expression> ']'
struct IndexExpr : Expr {
  Token braces[2];
  u32 expr;
};

// <function_name> '(' <expressions> ',' ... ')'
struct InvokeExpr : Expr {
  Token braces[2];
  Token commas[max::FUNC_ARGS - 2];
  u32 args[max::FUNC_ARGS];
  u32 func;
};

// <expression> <operation>
struct PostExpr : Expr {
  Token op;
  u32 expr;
};

// <expression> '?' <expression> ':' <expression>
struct TernaryExpr : Expr {
  Token query;
  Token colon;
  u32 expr[3];
};

// '(' <typename> ')' <expression>
struct CastExpr : Expr {
  Token braces[2];
  Token type;
  u32 expr;
};

// '(' <expr> ')'
struct NestedExpr : Expr {
  Token braces[2];
  u32 depth;
  u32 expr;
};

// '{' <head> '}'
struct CompoundStmt : Stmt {
  Token braces[2];
  u32 head;
};

// <kw> '(' <expr> ')' <head>
struct CondStmt : Stmt {
  Token kw;
  Token braces[2];
  u32 expr;
  u32 head;
};

// <kw> '(' <expr> ')' <head>
struct LoopStmt : CondStmt {};

// <kw> <expr>?
struct JumpStmt : Stmt {
  Token kw;
  u32 expr;
};

// Head definition:
// <type> <name> ('=' <expression>)? ','? <next_definition>
// Edges definition:
// <name> ('=' <expression>)? ','? <next_definition>
struct VarStmt : Stmt {
  Token type;
  Token assign;
  Token comma;
  u32 var;
  u32 expr;
  u32 next_def;
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
