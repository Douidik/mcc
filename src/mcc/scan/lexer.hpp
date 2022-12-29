#ifndef MCC_LEXER_HPP
#define MCC_LEXER_HPP

#include "syntax_map.hpp"
#include "token.hpp"

namespace mcc {

class Lexer {
public:
  Lexer(std::string_view src, SyntaxMap map = syntax_ansi());
  auto tokenize() -> Token;

private:
  auto match() -> Token;
  auto exception(std::string_view desc, Token token) -> Exception;
  auto dummy_token() -> Token;
  
  SyntaxMap m_map;
  std::string_view m_src;
  std::string_view m_next;
};

}  // namespace mcc

#endif
