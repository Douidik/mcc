#ifndef MCC_AST_PARSER_HPP
#define MCC_AST_PARSER_HPP

#include <optional>
#include "scan/token.hpp"
#include <queue>

namespace mcc {
class Lexer;
}  // namespace mcc

namespace mcc::ast {
class Ast;

class Parser {
public:
  Parser(Ast &ast, Lexer &lexer);
  auto parse() -> u32;

private:
  auto parse_type(Token &token) -> Token &;
  auto parse_def(Token type) -> u32;
  auto parse_var(Token type, Token identifier, Token op) -> u32;
  auto parse_func(Token type, Token identifier, Token braces) -> u32;

  auto next_token() -> Token;
  auto maybe_token(u32 cs = 0, u32 gp = 0, u32 n = 0) -> std::optional<Token>;
  auto expect_token(u32 cs = 0, u32 gp = 0, u32 n = 0) -> Token;
  auto peek_token(u32 cs = 0, u32 gp = 0, u32 n = 0) -> Token;
  auto peek_pop() -> Token;

  auto unexpected(std::string_view desc, u32 have, u32 expected) -> Exception;
  auto exception(std::string_view desc, Token token) -> Exception;

  Ast &m_ast;
  Lexer &m_lexer;
  std::queue<Token> m_peek_queue;
};

};  // namespace mcc::ast

#endif
