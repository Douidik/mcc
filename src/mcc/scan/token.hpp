#ifndef MCC_TOKEN_HPP
#define MCC_TOKEN_HPP

#include "trait.hpp"

namespace mcc {

struct Token {
  auto merge(Token token) const -> Token {
    if (!src.empty() and trait & trait::OK_MASK) {
      const char *begin = std::min(src.begin(), token.src.begin());
      const char *end   = std::max(src.end(), token.src.end());
      return Token{std::string_view{begin, end}, trait | token.trait};
    }
    return token;
  }

  auto ok() const -> bool {
    return trait & trait::OK_MASK;
  }

  operator bool() const {
    return trait & trait::OK_MASK;
  }

  std::string_view src;
  u32 trait;
};

}  // namespace mcc

#endif
