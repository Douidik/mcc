#include "code_exception.hpp"
#include <algorithm>

namespace mcc {

auto code_exception(std::string_view name, std::string_view desc, std::string_view src, Token token)
  -> Exception {
  auto line = std::count(src.begin(), token.expr.begin(), '\n');
  auto rbegin = std::find(token.expr.rend(), src.rend(), '\n');
  auto begin = std::max(rbegin.base(), src.begin());
  auto end = std::find(token.expr.end(), src.end(), '\n');
  auto cursor = token.expr.begin() - begin + 1;

  return Exception{
    "source code exception",
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

}  // namespace mcc
