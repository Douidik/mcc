#include "code_exception.hpp"
#include <algorithm>

namespace mcc {

  // TODO: support multiline code exceptions
auto code_exception(std::string_view name, std::string_view desc, std::string_view src, Token token)
  -> Exception {
  auto line = std::count(src.begin(), token.view.begin(), '\n');
  auto rbegin = std::find(token.view.rend(), src.rend(), '\n');
  auto begin = std::max(rbegin.base(), src.begin());
  auto end = std::find(token.view.end(), src.end(), '\n');
  auto cursor = token.view.begin() - begin + 1;

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
    token.view.size(),
    desc,
  };
}

}  // namespace mcc
