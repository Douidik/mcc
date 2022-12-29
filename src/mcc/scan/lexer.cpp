#include "lexer.hpp"
#include "trait.hpp"
#include <algorithm>

namespace mcc {
using namespace trait;

Lexer::Lexer(std::string_view src, SyntaxMap map) : m_src(src), m_next(src), m_map(map) {
  if (!m_src.ends_with('\n')) {
    throw exception("source does not ends with an endline character '\\n'", dummy_token());
  }
}

auto Lexer::tokenize() -> Token {
  auto token = match();

  if (token.trait & CsCatch) {
    throw exception(trait_catch_desc(token.trait), token);
  }

  return token.trait != Blank ? token : tokenize();
}

auto Lexer::match() -> Token {
  if (m_next.empty()) {
    return Token{{m_src.end(), m_src.end()}, End};
  }

  for (const auto &[trait, regex] : m_map) {
    if (auto match = regex.match(m_next)) {
      m_next = match.next();
      return Token{match.view(), trait};
    }
  }

  throw exception("unreachable, none should match everything", dummy_token());
}

auto Lexer::exception(std::string_view desc, Token token) -> Exception {
  auto line = std::count(m_src.begin(), token.expr.begin(), '\n');
  auto rbegin = std::find(token.expr.rend(), m_src.rend(), '\n');
  auto begin = std::max(rbegin.base(), m_src.begin());
  auto end = std::find(token.expr.end(), m_src.end(), '\n');
  auto cursor = token.expr.begin() - begin + 1;

  return Exception{
    "lexer exception",
    R"(with {{
  {} | {}
     {:>{}}{:^>{}} {}
}})",
    line,
    std::string_view{begin, end},
    "",
    cursor,
    "",
    token.expr.size(),
    desc,
  };
}

auto Lexer::dummy_token() -> Token {
  return {{&m_src.back(), 1}, End};
}

}  // namespace mcc
