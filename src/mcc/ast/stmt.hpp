#ifndef MCC_AST_STMT_HPP
#define MCC_AST_STMT_HPP

#include "node.hpp"
  #include "scan/token.hpp"
#include <forward_list>

namespace mcc::ast {
  
struct Stmt : Node {};

// '{' <stmt>, ... '}'
struct CompoundStmt : Stmt {
  Token braces[2];
  std::forward_list<Node *> body;
};

// <kw> '(' <expr> ')' <body>
struct CondStmt : Stmt {
  Token kw;
  Token braces[2];
  struct Expr *expr;
  CompoundStmt *body;
};

// <kw> '(' <expr> ')' <body>
struct LoopStmt : CondStmt {};

// <kw> <expr>?
struct JumpStmt : Stmt {
  Token kw;
  struct Expr *expr;
};

// Head definition:
// <type> <name> ('=' <expression>)? ';' | ','  <next_definition>
// Edges definition:
// <name> ('=' <expression>)? ','? <next_definition>
struct VarStmt : Stmt {
  struct Var *var;
  Token assign;
  Token comma;
  struct Expr *expr;
  VarStmt *next;
};

// <type> <name> '(' <args> ')' '{' <body> '}'
struct FuncStmt : Stmt {
  struct Func *func;
  Token paren[2];
  Token curly[2];
  CompoundStmt *body;
};

struct TypedefStmt : Stmt {
  struct Typedef *def;
  Token kw;
};

}  // namespace mcc::ast

#endif
