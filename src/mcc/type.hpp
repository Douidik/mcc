#ifndef MCC_TYPE_HPP
#define MCC_TYPE_HPP

#include "trait.hpp"

namespace mcc {

struct Type {
  enum Modifier : u32 {
    Long     = KwLong & TYPE_MASK,
    Short    = KwShort & TYPE_MASK,
    Volatile = KwVolatile & TYPE_MASK,
    Const    = KwConst & TYPE_MASK,
    Extern   = KwExtern & TYPE_MASK,
    Register = KwRegister & TYPE_MASK,
    Static   = KwStatic & TYPE_MASK,
    Signed   = KwSigned & TYPE_MASK,
    Unsigned = KwUnsigned & TYPE_MASK
  };

  auto ok() const -> bool {
    return mode != 0 and defn != 0;
  }

  u32 mode;
  struct Defn *defn;
};

}  // namespace mcc

#endif
