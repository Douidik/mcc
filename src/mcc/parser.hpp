#ifndef MCC_PARSER_HPP
#define MCC_PARSER_HPP

#include "ast.hpp"
#include "scan/lexer.hpp"
#include <deque>

namespace mcc {

class Parser {
public:
  Parser(Lexer lexer);
  auto parse() -> Ast &;

private:
  auto parse_type() -> Type;
  auto parse_defn() -> struct Stmt *;
  auto parse_expr(struct Expr *stack = {}) -> struct Expr *;
  auto parse_stmt() -> struct Stmt *;

  auto parse_func(Type type, Token name) -> struct FuncStmt *;
  auto parse_param(std::span<struct Var *> params, size_t n) -> std::span<struct Var *>;
  auto parse_init(Type type, Token name) -> struct InitStmt *;

  auto parse_argument(std::span<struct Expr *> args, size_t n) -> std::span<struct Expr *>; 
  auto parse_compound_stmt() -> struct CompoundStmt *;
  
  auto token_next() -> Token;
  auto token_expect(u32 mask) -> Token;
  auto token_maybe(u32 mask) -> Token;
  auto token_peek() -> Token;
  void token_peek_erase();
  auto token_peek_pop() -> Token;
  auto token_peek_top() -> Token;
  auto token_peek_front() -> Token;

  auto exception(std::string_view fmt, Token token, auto... args) -> Exception;

  Ast m_ast;
  Lexer m_lexer;
  std::deque<Token> m_peek_deque;
};

}  // namespace mcc

#endif
