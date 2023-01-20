#ifndef MCC_NODE_HPP
#define MCC_NODE_HPP

namespace mcc {

// TODO: define virtual functions for code generation
// e.g: virtual auto asm_x86(AsmContext *ctx) const -> AsmContext *;
class Node {
public:
  virtual auto graph(class GraphContext &ctx) -> class GraphContext & = 0;
  virtual auto asm_x86(class AsmContext &ctx) -> class AsmContext   & = 0;
};

}  // namespace mcc

#endif
