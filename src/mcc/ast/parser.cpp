#include "parser.hpp"
#include "ast.hpp"
#include "code_exception.hpp"
#include "scan/lexer.hpp"

namespace mcc::ast {

Parser::Parser(Ast &ast, Lexer &lexer) : m_lexer(lexer), m_ast(ast) {}

auto Parser::parse() -> u32 {
  auto token = expect_token();

  if (token.trait & GpType) {
    auto type = parse_type(token);
    return parse_def(token);
  }

  return none();
}

auto Parser::parse_type(Token &token) -> Token & {
  auto new_token = peek_token();

  if (new_token.trait & GpType) {
    // Combine types
    token.expr = std::string_view{
      token.expr.begin(),
      new_token.expr.end(),
    };
    token.trait = token.trait | new_token.trait;

    peek_pop();
    parse_type(token);
  }

  return token;
}

auto Parser::parse_def(Token type) -> u32 {
  auto identifier = expect_token(CsIdentifier);
  
  if (auto assign = maybe_token(CsOperator, GpBinaryOp, 0)) {
    return parse_var(type, identifier, *assign);
  }

  if (auto brace = maybe_token(CsOperator, GpBracket, 2)) {
    return parse_func(type, identifier, *brace);
  }

  throw exception("unexpected in definition", next_token());
}

auto Parser::parse_var(Token type, Token identifier, Token op) -> u32 {
  
  
  u32 var = m_ast.push(Var{type, assign});

  if (auto end = maybe_token(CsPunctuator, GpNone, 3))
    return m_ast.push(VarStmt{type, assign
  if (auto comma = maybe_token(CsPunctuator, GpNone, 1)) {
    return m_ast.push(VarStmt{type, assign, *comma, var});
  }
}

auto parse_func(Token type, Token identifier, Token brace) -> u32 {}

auto Parser::next_token() -> Token {
  if (!m_peek_queue.empty()) {
    return peek_pop();
  }

  return m_lexer.tokenize();
}

auto maybe_token(u32 cs = 0, u32 gp = 0, u32 n = 0) -> std::optional<Token> {
  auto token = peek_token();
  auto &[expr, trait] = token;

  if (n != 0 and !(trait & n) or gp != 0 and !(trait & gp) or cs != 0 and !(trait & cs)) {
    return {};
  }

  return peek_pop();
}

auto Parser::expect_token(u32 cs, u32 gp, u32 n) -> Token {
  auto token = next_token();
  auto &[expr, trait] = token;

  if (n != 0 and !(trait & n)) {
    throw exception("unexpected token (cs | gp | n)", token);
  }

  if (gp != 0 and !(trait & gp)) {
    throw exception("unexpected token of group", token);
  }

  if (cs != 0 and !(trait & cs)) {
    throw exception("unexpected token of class", token);
  }

  return token;
}

auto Parser::peek_token(u32 cs, u32 gp, u32 n) -> Token {
  return m_peek_queue.emplace(expect_token(cs, gp, n));
}

auto Parser::peek_pop() -> Token {
  if (m_peek_queue.empty()) {
    throw exception("Peek queue is empty", m_lexer.dummy_token());
  }

  Token token = m_peek_queue.front();
  return m_peek_queue.pop(), token;
}

auto unexpected(std::string_view desc, u32 have, u32 expected) -> Exception {
  auto fmt = "Unexpected '{}' <{}>, must be ";
  return code_exception("parser exception", "unexpected");
}

auto Parser::exception(std::string_view desc, Token token) -> Exception {
  return code_exception("parser exception", desc, m_lexer.src(), token);
}

}  // namespace mcc::ast
