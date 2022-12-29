#include "regex/format.hpp"
#include "scan/lexer.hpp"
#include <fmt/format.h>
#include <fstream>

using namespace mcc::literals;

// int main(int argc, char **argv) {
//   auto regex = "{' '} ~ 'sus'"_rx;
  
//   // auto regex = "'#' !{'\n'}*"_rx;
//   fmt::print("{}\n", regex);

// //   auto expr = R"("
// // #define new(n)\
// // malloc(n)
// // ")";

//   auto expr = R"("Hello world\"" //the next expression)";
//   auto match = regex.match(expr);
//   fmt::print("/* {} => {} */", match.expr(), match.view());
// }

// void dump_tokens(mcc::Lexer &lexer) {
//   auto [expr, trait] = lexer.tokenize();
//   fmt::print("{:>30} : {}\n", expr, mcc::trait_name(trait));

//   if (trait != mcc::trait::End) {
//     dump_tokens(lexer);
//   }
// }

// int main(int argc, char **argv) {
//   if (argc < 2) {
//     fmt::print("mcc - micro c compiler\n");
//     return 0;
//   }

//   try {
//     std::fstream fstream{argv[1]};
//     if (!fstream) {
//       throw mcc::Exception{"fs exception", "can't read file from '{}'", argv[1]};
//     }
//     std::string src{
//       std::istreambuf_iterator<char>(fstream),
//       std::istreambuf_iterator<char>(),
//     };

//     mcc::Lexer lexer{src, mcc::syntax_ansi()};
//     dump_tokens(lexer);
//   } catch (const mcc::Exception &exception) {
//     fmt::print(stderr, "Mcc {} {}\n", exception.name(), exception.what());
//   } catch (const std::exception &exception) {
//     fmt::print(stderr, "{}\n", exception.what());
//   }
// }

#include "regex/format.hpp"
#include "scan/syntax_map.hpp"

namespace mcc {

void machine() {
  size_t i = 0;
  for (const auto &[trait, regex] : syntax_ansi()) {
    auto filepath = fmt::format("graphs/{}{}.graph", i++, trait_name(trait));
    FILE *fp = fopen(filepath.data(), "w");
    if (!fp) {
      throw Exception("machine", "can't open filestream w to '{}'", filepath);
    }
    auto fmt = fmt::format("{{:{}}}\n", trait_name(trait));
    fmt::print(fp, fmt::runtime(fmt), regex);
    fclose(fp);
  }
}

}  // namespace mcc

int main(int argc, char **argv) {
  mcc::machine();
}
