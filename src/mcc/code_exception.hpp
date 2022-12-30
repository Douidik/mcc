#ifndef MCC_CODE_EXCEPTION_HPP
#define MCC_CODE_EXCEPTION_HPP

#include "mcc.hpp"
#include "scan/token.hpp"

namespace mcc {

auto code_exception(std::string_view name, std::string_view desc, std::string_view src, Token token)
  -> Exception;

}

#endif
