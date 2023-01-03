#ifndef MCC_TOKEN_HPP
#define MCC_TOKEN_HPP

#include "mcc.hpp"

namespace mcc {

struct Token {
  operator bool() const {
    return static_cast<bool>(trait);
  }

  auto merge(Token token) -> Token {
    if (token.view != "" and !trait) {
      const char *begin = std::min(view.begin(), token.view.begin());
      const char *end   = std::max(view.end(), token.view.end());
      return Token{std::string_view{begin, end}, trait | token.trait};
    }
    return token;
  }

  std::string_view view;
  u32 trait;
};

}  // namespace mcc

#endif
