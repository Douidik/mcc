#ifndef MCC_AST_EXPR_HPP
#define MCC_AST_EXPR_HPP

#include "limits.hpp"
#include "node.hpp"
#include "scan/token.hpp"

namespace mcc::ast {

struct Expr : Node {};

struct IdentifierExpr : Expr {
  struct Defn *defn;
};

struct ConstantExpr : Expr {
  Token value;
};

enum class Order {
  Pre,
  Post,
};

// <operation><expression>
struct UnaryExpr : Expr {
  Token op;
  Node *expr;
  Order order;
};

// <expression[0]><operation><expression[1]>
struct BinaryExpr : Expr {
  Node *a;
  Token op;
  Node *b;
};

// '[' <expression> ']'
struct IndexExpr : Expr {
  Token braces[2];
  Node *expr;
};

// <function_name> '(' <expressions> ',' ... ')'
struct InvokeExpr : Expr {
  Token braces[2];
  Token commas[max::FUNC_ARGS - 2];
  Expr *args[max::FUNC_ARGS];
  Node *func;
};

// <expression> '?' <expression> ':' <expression>
struct TernaryExpr : Expr {
  Token query;
  Token colon;
  Node *expr[3];
};

// '(' <typename> ')' <expression>
struct CastExpr : Expr {
  Token braces[2];
  Token type;
  Node *expr;
};

// '(' <expr> ')'
struct NestedExpr : Expr {
  Token braces[2];
  Node *expr;
};

}  // namespace mcc::ast

#endif
