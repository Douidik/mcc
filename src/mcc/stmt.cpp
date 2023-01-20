#include "stmt.hpp"
#include "asm_context.hpp"
#include "graph_context.hpp"

namespace mcc {

auto MainStmt::graph(GraphContext &ctx) -> GraphContext & {}

auto MainStmt::asm_x86(AsmContext &ctx) -> AsmContext & {}

}  // namespace mcc
