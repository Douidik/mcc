#include "parser.hpp"
#include "defn.hpp"
#include "expr.hpp"
#include "stmt.hpp"

namespace mcc {

Parser::Parser(Lexer lexer) : m_lexer(lexer) {
  // Define basic types in the ast
  m_ast.defn(new auto(Primitive::defn_void()));
  m_ast.defn(new auto(Primitive::defn_char()));
  m_ast.defn(new auto(Primitive::defn_short()));
  m_ast.defn(new auto(Primitive::defn_int()));
  m_ast.defn(new auto(Primitive::defn_long()));
  m_ast.defn(new auto(Primitive::defn_float()));
  m_ast.defn(new auto(Primitive::defn_double()));
  m_ast.defn(new auto(Primitive::defn_signed()));
  m_ast.defn(new auto(Primitive::defn_unsigned()));
}

auto Parser::parse() -> Ast & {
  parse_stmt();
  return m_ast;
}

auto Parser::parse_type() -> Type {
  using enum DefnKind;

  for (Type type{};;) {
    if (auto name = token_maybe(CsIdentifier | GpPrimitive)) {
      // NOTE: Too far, the token can be the name of the identifier
      // We have to check past the type defn because there is no order
      // in the type declaration, e.g: const int a -> int const a;
      if (type.defn != nullptr) {
        return type;
      }
      type.defn = m_ast.find(name.src);

      if (type.defn and type.defn->kind() != Primitive and type.defn->kind() != Struct) {
        throw exception("identifier does not refer to a defined type", name);
      }
    } else if (auto mode = token_maybe(GpModifier)) {
      // TODO: choose to define type mode as a bitfield or as a compound token
      type.mode |= (mode.trait & TYPE_MASK);

      if (type.mode & mode.trait) {
        throw exception("repeated type modifier in declaration", mode);
      }
      if (type.mode & Type::Signed and type.mode & Type::Unsigned) {
        throw exception("cannot combine type modifiers of discordant signedness", mode);
      }
    } else {
      // NOTE: free the node queue if the token
      if (type.ok()) token_peek_erase();
      return type;
    }
  }
}

auto Parser::parse_defn() -> Stmt * {
  auto peek = token_peek();

  // var-defn | func-defn
  if (auto type = parse_type(); type.ok()) {
    auto name = token_expect(CsIdentifier);
    auto next = token_peek();

    if (next.trait != ParenBegin) {
      return parse_init(type, name);
    } else {
      return parse_func(type, name);
    }
  }

  return {};
}

// TODO: write an intermediate func to compose expressions
auto Parser::parse_expr(Expr *stack) -> Expr * {
  if (auto id = token_maybe(CsIdentifier)) {
    auto defn = m_ast.find(id.src);
    auto next = token_peek();

    if (!defn) {
      throw exception("unknown indentifier in expression", id);  // ?:J
    }

    // NOTE: try to invoke the definition
    if (next.trait & ParenBegin) {
      if (defn->kind() != DefnKind::Func) {
        throw exception("cannot invoke non-function expression", id);
      }
      std::array<Expr *, max::func_args> args{0};
      return m_ast.push(new InvokeExpr{static_cast<Func *>(defn), parse_argument(args, 0)});
    }

    auto expr = m_ast.push(new IdExpr{m_ast.find(id.src)});
  }

  if (auto string = token_maybe(String)) {
    m_ast.defn(new StringConstant{string});
    return m_ast.push(new ConstantExpr{string});
  }

  if (auto constant = token_maybe(CsConstant)) {
    return m_ast.push(new ConstantExpr{constant});
  }

  if (auto sign = token_maybe(Add | Sub); sign.ok() and !stack) {
    return m_ast.push(new UnaryExpr{Order::Prev, parse_expr(), sign});
  }

  if (auto binary_op = token_maybe(CsOperator)) {
    return m_ast.push(new BinaryExpr{stack, parse_expr(), binary_op});
  }

  //                 _
  // :monkey_jam: @('-')@
  //             _-*| |._-
  if (auto increment_op = token_maybe(Increment | Decrement)) {
    if (!stack) {
      return m_ast.push(new UnaryExpr{Order::Prev, stack, increment_op});
    } else {
      return m_ast.push(new UnaryExpr{Order::Post, parse_expr(), increment_op});
    }
  }

  // cast-expr | nested-expr
  if (token_maybe(ParenBegin)) {
    if (auto type = parse_type(); type.ok()) {
      return m_ast.push(new CastExpr{type, parse_expr()});
    } else {
      // TODO: nested-expr case L:.|
    }
  }

  throw exception("expected expression", token_next());
}

auto Parser::parse_stmt() -> Stmt * {
  auto token = token_peek();

  if (auto type = parse_type(); type.ok()) {
    return parse_defn();
  }

  return {};
}

auto Parser::parse_func(Type type, Token name) -> FuncStmt * {
  auto params = std::array<Var *, max::func_args>{0};
  auto func   = m_ast.defn(new Func{type, name.src, parse_param(params, 0)});
  auto next   = token_peek();

  if (token_maybe(Semicolon)) {
    return m_ast.push(new FuncStmt{func, {}});
  } else {
    return m_ast.push(new FuncStmt{func, parse_compound_stmt()});
  }
}

auto Parser::parse_param(std::span<Var *> params, size_t n) -> std::span<struct Var *> {
  if (!n) {
    token_expect(ParenBegin);
  }

  // NOTE: Are anonymous parameters defined in the ansi standard ? Warning ?
  auto type = parse_type();
  auto name = token_maybe(CsIdentifier);
  m_ast.push(new Var{type, name.ok() ? name.src : m_lexer.dummy_token().src});

  if (token_maybe(Comma)) {
    return parse_param(params, n + 1);
  } else {
    token_expect(ParenClose);
    return params.subspan(0, n + 1);
  }
}

auto Parser::parse_init(Type type, Token name) -> struct InitStmt * {
  Var *var   = m_ast.defn(new Var{type, name.src});
  Expr *expr = nullptr;

  if (auto assign = token_maybe(Assign)) {
    if (expr = parse_expr(); !expr) {
      throw exception("expected expression after assignment", assign);
    }
  }

  if (auto comma = token_maybe(Comma)) {
    parse_init(type, token_expect(CsIdentifier));
  } else {
    token_expect(Semicolon);
  }

  return m_ast.push(new InitStmt{var, expr});
}

auto Parser::parse_argument(std::span<Expr *> args, size_t n) -> std::span<Expr *> {
  if (!n) {
    token_expect(ParenBegin);
  }

  // NOTE: expression parsing should stop when encoutering a comma or a closing-parenthesis
  args[n] = parse_expr();

  if (token_maybe(Comma)) {
    return parse_argument(args, n + 1);
  } else {
    token_expect(ParenClose);
    return args.subspan(0, n + 1);  // NOTE: n-included ?
  }
}

auto Parser::token_next() -> Token {
  if (!m_peek_deque.empty()) {
    return token_peek_pop();
  }

  return m_lexer.tokenize();
}

auto Parser::token_expect(u32 mask) -> Token {
  auto token = token_next();

  if (u32 type = trait_type(mask); !(token.trait & type)) {
    throw exception("todo! type based error", token);
  }

  if (!(token.trait & mask)) {
    throw exception("todo! mask based error", token);
  }

  return token;
}

auto Parser::token_maybe(u32 mask) -> Token {
  auto token = token_peek_front();

  if (!(token.trait & trait_type(mask)) or !(token.trait & mask)) {
    token.trait &= ~OK_MASK;
  }

  return token;
}

auto Parser::token_peek() -> Token {
  return m_peek_deque.emplace_back(m_lexer.tokenize());
}

void Parser::token_peek_erase() {
  m_peek_deque.clear();
}

auto Parser::token_peek_pop() -> Token {
  Token token = token_peek_front();
  m_peek_deque.pop_front();
  return token;
}

auto Parser::token_peek_front() -> Token {
  if (!m_peek_deque.empty()) {
    return token_peek();
  }

  return m_peek_deque.front();
}

}  // namespace mcc
