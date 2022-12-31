#ifndef MCC_AST_PARSER_HPP
#define MCC_AST_PARSER_HPP

#include "ast.hpp"
#include "scan/token.hpp"
#include <optional>
#include <queue>
#include <stack>

namespace mcc {
class Lexer;
}  // namespace mcc

namespace mcc::ast {

class Parser {
public:
  Parser(Ast &ast, Lexer &lexer);
  auto parse() -> Node *;

private:
  auto parse_type(Token &token) -> Token &;
  auto parse_def(Token type) -> Node *;
  auto parse_var(Token type, Token name, Token assign) -> Node *;
  auto parse_func(Token type, Token name, Token braces) -> Node *;

  auto parse_expr() -> Node *;

  auto next_token() -> Token;

  auto maybe_token(u32 trait = 0) -> std::optional<Token>;
  auto expect_token(u32 trait = 0) -> Token;
  auto peek_token(u32 trait = 0) -> Token;

  auto peek_pop() -> Token;

  auto unexpected(std::string_view desc, u32 have, u32 expected) -> Exception;
  auto exception(std::string_view desc, Token token) -> Exception;

  // template<typename T>
  // auto ast(u32 n) -> const T & {
  //   throw u32{};
  //   // return m_ast.nodes().front();
  // }

  Ast &m_ast;
  Lexer &m_lexer;
  std::stack<u32> m_expr_stack;
  std::queue<Token> m_peek_queue;
};

};  // namespace mcc::ast

#endif
