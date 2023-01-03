#include "parser.hpp"
#include "code_exception.hpp"
#include "expr.hpp"
#include "scan/lexer.hpp"
#include "stmt.hpp"
#include <charconv>
#include <variant>

namespace mcc::ast {

Parser::Parser(Ast &ast, Lexer &lexer) : m_lexer(lexer), m_ast(ast) {}

auto Parser::parse() -> Node * {
  auto [view, trait] = peek_token();

  if (trait == End) {
    return {};
  }

  if (trait & GpType) {
    return parse_def();
  }

  m_peek_queue.pop_front();
  return parse();
}

auto Parser::parse_type() -> Type {
  using enum DefnKind;
  Type type{};

  for (;;) {
    auto token = peek_token();

    if (token.trait & (CsIdentifier | GpType)) {
      if (type.defn != nullptr) {
        return type;
      }

      if (token.trait & CsIdentifier) {
        auto defined = m_ast.resolve(token);

        if (defined->kind() != Func and defined->kind() != Enum and defined->kind() != Struct) {
        }
      } else {
        // NOTE: this primitives should be static !
        auto primitive  = m_ast.define(new Primitive{});
        primitive->type = Primitive::Type{token.trait & TYPE_MASK};
        primitive->name = token;
        type.defn       = primitive;
      }

      type.name = token;
    }

    if (token.trait & GpModifier) {
      type.mods = type.mods.merge(token);
    }

    expect_token(CsIdentifier | GpType | GpModifier);
  }

  return type;

  // auto type = expect_token(GpType);
  // auto spec = Token{};

  // for (;;) {
  //   if (spec = maybe_token(GpType); !spec) break;

  //   if (spec.trait & GpType and (spec.trait & type.trait & TYPE_MASK)) {
  //     throw exception("Repeated specifier in type declaration", spec);
  //   }

  //   type = type.merge(spec);

  //   if (type.trait & TYPE_MASK & KwSigned and type.trait & TYPE_MASK & KwUnsigned) {
  //     // throw unexpected(type, spec.trait);
  //     throw exception("Cannot combine type specifiers of discordant signedness", spec);
  //   }
  // }

  // return type;
}

auto Parser::parse_def() -> Node * {
  auto type = parse_type();
  auto name = expect_token(Identifier);

  if (auto brace = maybe_token(ParenBegin)) {
    return parse_func(type, name, brace);
  } else {
    return parse_var(type, name);
  }
}

auto Parser::parse_var(Type type, Token name) -> VarStmt * {
  auto var  = m_ast.define(new Var{});
  auto stmt = m_ast.push(new VarStmt{});

  var->name = name;
  var->type = type;
  stmt->var = var;

  if (auto assign = maybe_token(Assign)) {
    stmt->assign = assign;
    stmt->expr   = parse_expr();
  }

  if (auto comma = maybe_token(Comma)) {
    stmt->comma = comma;
    stmt->next  = parse_var(type, expect_token(Identifier));
  }

  return stmt;
}

auto Parser::parse_func(Type type, Token name, Token brace) -> FuncStmt * {
  return {};
}

auto Parser::parse_expr(Expr *prev) -> Expr * {
  if (auto constant = maybe_token(CsConstant)) {
    auto expr   = m_ast.push(new ConstantExpr{});
    expr->value = constant;
    return parse_expr(expr);
  }

  if (auto identifier = maybe_token(Identifier)) {
    auto defn = m_ast.resolve(identifier);
    if (!defn) {
      throw exception("Use of unresolved identifier", identifier);
    }

    auto expr  = m_ast.push(new IdentifierExpr{});
    expr->defn = defn;
    return parse_expr(expr);
  }

  if (auto sign = maybe_token(Add | Sub); sign and !prev) {
    auto unary   = m_ast.push(new UnaryExpr{});
    unary->op    = sign;
    unary->expr  = parse_expr(unary);
    unary->order = Order::Post;
    if (!unary->expr) {
      throw exception("expected expression after sign", sign);
    }
    return parse_expr(unary);
  }

  if (auto binary_op = maybe_token(GpBinaryOp)) {
    auto binary = m_ast.push(new BinaryExpr{});
    binary->a   = prev;
    binary->b   = parse_expr(binary);
    binary->op  = binary_op;

    if (!binary->a) {
      throw exception("missing left-handed expression", binary_op);
    }
    if (!binary->b) {
      throw exception("missing right-handed expression", binary_op);
    }

    return parse_expr(binary);
  }

  if (auto increment_op = maybe_token(Increment | Decrement)) {
    auto increment = m_ast.push(new UnaryExpr{});

    increment->op = increment_op;
    if (prev) {
      increment->expr  = prev;
      increment->order = Order::Pre;
    } else {
      increment->expr  = parse_expr(increment);
      increment->order = Order::Post;
    }

    if (!increment->expr) {
      throw exception("missing operand for increment", increment_op);
    }
    return increment;
  }

  if (auto subscript_begin = maybe_token(CrochetBegin)) {
    auto index       = m_ast.push(new IndexExpr{});
    index->expr      = parse_expr();
    index->braces[0] = subscript_begin;
    index->braces[1] = expect_token(CrochetClose);

    if (!index->expr) {
      throw exception("missing expression in subscript operator", index->braces[0]);
    }
    return index;
  }

  if (auto nested_begin = maybe_token(ParenBegin)) {
    return parse_expr();
  }

  return {};
}

auto Parser::next_token() -> Token {
  if (!m_peek_queue.empty()) {
    Token peek = m_peek_queue.front();
    return m_peek_queue.pop_front(), peek;
  }

  return m_lexer.tokenize();
}

auto Parser::peek_token() -> Token {
  return m_peek_queue.emplace_back(m_lexer.tokenize());
}

auto Parser::maybe_token(u32 wanted) -> Token {
  auto token          = next_token();
  auto [view, trait]  = token;
  auto [cs, gp, type] = trait_decompose(wanted);

  if (type != 0 and !(trait & type) or gp != 0 and !(trait & gp) or cs != 0 and !(trait & cs)) {
    m_peek_queue.push_front(token);
    token.trait = None;
  }

  return token;
}

auto Parser::expect_token(u32 wanted) -> Token {
  auto token          = next_token();
  auto [view, trait]  = token;
  auto [cs, gp, type] = trait_decompose(wanted);

  if (type != 0 and !(trait & type) or gp != 0 and !(trait & gp) or cs != 0 and !(trait & cs)) {
    unexpected(token, wanted);
  }

  return token;
}

}  // namespace mcc::ast
