#ifndef MCC_RAW_STRING_HPP
#define MCC_RAW_STRING_HPP

#include "mcc.hpp"

namespace mcc {

struct Raw {
  std::string_view string;
  u32 escape_depth;
};

}  // namespace mcc

namespace fmt {
using namespace mcc;

template<>
struct formatter<Raw> {
  constexpr auto parse(format_parse_context &ctx) {
    return ctx.begin();
  }

  constexpr auto format_escape_sequence(std::string_view sequence, size_t depth, auto &context) {
    for (size_t i = 0; i < depth; i++) {
      format_to(context.out(), "\\");
    }

    return format_to(context.out(), "{}", sequence);
  }

  constexpr auto format_char(const char &c, size_t depth, auto &context) {
    switch (c) {
    case '\t': return format_escape_sequence("t", depth, context);
    case '\v': return format_escape_sequence("v", depth, context);
    case '\0': return format_escape_sequence("0", depth, context);
    case '\b': return format_escape_sequence("b", depth, context);
    case '\f': return format_escape_sequence("f", depth, context);
    case '\n': return format_escape_sequence("n", depth, context);
    case '\r': return format_escape_sequence("r", depth, context);
    case '\\': return format_escape_sequence("\\", depth, context);
    case '\"': return format_escape_sequence("\\\"", depth, context);

    default: return format_to(context.out(), "{}", c);
    }
  }

  constexpr auto format(const Raw &raw, auto &ctx) {
    for (const char &c : raw.string) {
      format_char(c, raw.escape_depth, ctx);
    }
    return ctx.out();
  }
};

}  // namespace fmt

#endif
