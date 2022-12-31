#ifndef MCC_TOKEN_HPP
#define MCC_TOKEN_HPP

#include "mcc.hpp"

namespace mcc {

struct Token {
  std::string_view view;
  u32 trait;
};

}  // namespace mcc

#endif
