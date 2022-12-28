#include "regex/format.hpp"
#include "regex/regex.hpp"

using namespace mcc::literals;

int main(int argc, char **argv) {
  // auto regex =
  //   "{'+' | '-'}*"
  //   "{{'0b'|'0B'} {[0-1]}+                     } |"
  //   "{{'0x'|'0X'} {[0-9]|[a-f]|[A-F]}+         } |"
  //   "{            {[0-9]}+                     } |"
  //   "a*"_rx;

  // auto match = regex.match(argv[1]);

  // fmt::print("{}\n", regex);
  // fmt::print("'{}' => '{}'\n", match.expr(), match.view());
  auto regex =
    "{'+' | '-'}*"
    "{{'0b'|'0B'} {[0-1]}*                     '.' {[0-1]}*                    } |"
    "{{'0x'|'0X'} {[0-9]|[a-f]|[A-F]}* '.' {[0-9]|[a-f]|[A-F]}*                } |"
    "{            {[0-9]}*                     '.' {[0-9]}*                    }  "
    "{{'e' | 'E'}  {'+' | '-'}? [0-9]+}?"
    "a*"_rx;
  // auto regex = "'a'* 'b'* a*"_rx;
  
  if (argc > 1) {
    auto match = regex.match(argv[1]);
    fmt::print("{} => {}\n", match.expr(), match.view());
  } else {
    fmt::print("{}\n", regex);
  }
}
