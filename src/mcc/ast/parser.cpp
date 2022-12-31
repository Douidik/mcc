#include "parser.hpp"
#include "code_exception.hpp"
#include "scan/lexer.hpp"
#include <charconv>

namespace mcc::ast {

Parser::Parser(Ast &ast, Lexer &lexer) : m_lexer(lexer), m_ast(ast) {}

auto Parser::parse() -> Node * {
  auto token = expect_token();

  if (token.trait & GpType) {
    auto type = parse_type(token);
    return parse_def(token);
  }

  // return none();
}

auto Parser::parse_type(Token &token) -> Token & {
  auto new_token = peek_token();

  if (token.trait & new_token.trait) {
    throw exception("Duplicate specifier in type declaration", new_token);
  }

  if (new_token.trait & GpType) {
    // Combine types
    token.view = std::string_view{
      token.view.begin(),
      new_token.view.end(),
    };
    token.trait |= new_token.trait;

    peek_pop();
    parse_type(token);
  }

  return token;
}

auto Parser::parse_def(Token type) -> Node * {
  auto name = expect_token(CsIdentifier);

  if (auto brace = maybe_token(ParenBegin)) {
    return parse_func(type, name, *brace);
  }

  if (auto assign = maybe_token(Assign)) {
    return parse_var(type, name, *assign);
  }

  throw exception("unexpected in definition", next_token());
}

auto Parser::parse_var(Token type, Token name, Token assign) -> Node * {
  // return m_ast.push(VarStmt{
  // type, assign if (auto comma = maybe_token(CsPunctuator, GpNone, 1)) {
  // return m_ast.push(VarStmt{type, assign, *comma, var});
  // }
  // u32m var = m_ast.push(Var{type, assign});

  // ast<VarStmt>(stmt).type = type;
  // ast<VarStmt>(stmt).var = m_ast.push<Var>(type, name);

  // if (auto end = maybe_token(CsPunctuator, GpNone, 3)) {
  //   return stmt;
  // }

  // if (auto assign = maybe_token(CsOperator, GpBinaryOp, 0)) {
  //   ast<VarStmt>(stmt).assign = *assign;
  //   ast<VarStmt>(stmt).expr = parse_expr();
  // }
  // return {};

  auto stmt = m_ast.push<VarStmt>(type);
  auto head = stmt.defs.push_front(&m_ast.push<Var>(type, name));
}

auto Parser::parse_func(Token type, Token name, Token brace) -> Node * {
  return {};
}

auto Parser::parse_expr() -> Node * {
  if (auto constant = maybe_token(CsConstant)) {
    return m_ast.push<Constant>(constant->view);
  }

  if (auto binary_op = maybe_token(CsOperator, GpBinaryOp)) {
  }  //return m_ast.push<BinaryExpr>(
}

// return none();
// throw exception("expression expected", next_token());
// }

auto Parser::next_token() -> Token {
  if (!m_peek_queue.empty()) {
    return peek_pop();
  }

  return m_lexer.tokenize();
}

auto Parser::maybe_token(u32 trait) -> std::optional<Token> {
  auto token = peek_token();
  auto [view, trait] = token;
  auto [cs, gp, type] = trait_decompose(trait);

  if (n != 0 and !(trait & type) or gp != 0 and !(trait & gp) or cs != 0 and !(trait & cs)) {
    return {};
  }

  return peek_pop();
}

auto Parser::expect_token(u32 trait) -> Token {
  auto token = peek_token();
  auto [view, trait] = token;
  auto [cs, gp, type] = trait_decompose(trait);

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

auto Parser::peek_token(u32 trait) -> Token {
  return m_peek_queue.emplace(expect_token(trait));
}

auto Parser::peek_pop() -> Token {
  if (m_peek_queue.empty()) {
    throw exception("Peek queue is empty", m_lexer.dummy_token());
  }

  Token token = m_peek_queue.front();
  return m_peek_queue.pop(), token;
}

auto Parser::unexpected(std::string_view desc, u32 have, u32 expected) -> Exception {
  auto fmt = "Unexpected '{}' <{}>, must be ";
  // return code_exception("parser exception", "unexpected");
}

auto Parser::exception(std::string_view desc, Token token) -> Exception {
  return code_exception("parser exception", desc, m_lexer.src(), token);
}

}  // namespace mcc::ast
