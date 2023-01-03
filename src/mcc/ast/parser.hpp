#ifndef MCC_AST_PARSER_HPP
#define MCC_AST_PARSER_HPP

#include "ast.hpp"
#include "code_exception.hpp"
#include "scan/lexer.hpp"
#include <deque>

namespace mcc::ast {

class Parser {
public:
  Parser(Ast &ast, Lexer &lexer);
  auto parse() -> Node *;

private:
  auto parse_type() -> Type;
  auto parse_def() -> Node *;
  auto parse_var(Type type, Token name) -> struct VarStmt *;
  auto parse_func(Type type, Token name, Token brace) -> struct FuncStmt *;

  auto parse_expr(struct Expr *prev = {}) -> struct Expr *;

  auto next_token() -> Token;
  auto peek_token() -> Token;
  auto maybe_token(u32 wanted = 0) -> Token;
  auto expect_token(u32 wanted = 0) -> Token;

  auto unexpected(Token token, u32 expected) const -> Exception {
    std::string report  = "expected token of type(s) |";
    auto [view, trait]  = token;
    auto [cs, gp, type] = trait_decompose(expected);

    auto report_push = [&report](std::string_view fmt, auto... args) {
      return fmt::format_to(std::back_inserter(report), fmt::runtime(fmt), args...);
    };

    if (type != 0 and !(trait & type)) {
      for (u32 i = 0, mask = 1; i < TYPE_SIZE; i++, mask <<= 1) {
        if (mask & expected) report_push("{}|", trait_type_desc(cs | gp | mask));
      }
    } else if (gp != 0 and !(trait & gp)) {
      for (u32 i = 0, mask = 1; i < GROUP_SIZE; i++, mask <<= 1) {
        if (mask & expected) report_push("{}|", trait_group_desc(gp));
      }
    } else if (cs != 0 and !(trait & cs)) {
      for (u32 i = 0, mask = 1; i < CLASS_SIZE; i++, mask <<= 1) {
        if (mask & expected) report_push("{}|", trait_class_desc(cs));
      }
    }

    return code_exception("parser exception", report, m_lexer.src(), token);
  }

  auto exception(std::string_view desc, Token token, auto... args) const -> Exception {
    return code_exception("parser exception", desc, m_lexer.src(), token, args...);
  }

  Ast &m_ast;
  Lexer &m_lexer;
  std::deque<Token> m_peek_queue;
};

};  // namespace mcc::ast

#endif
