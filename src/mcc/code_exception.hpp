#ifndef MCC_CODE_EXCEPTION_HPP
#define MCC_CODE_EXCEPTION_HPP

#include "mcc.hpp"
#include "scan/token.hpp"

namespace mcc {

class CodeException {
public:
  CodeException(std::string_view name, std::string_view fmt, auto... args) {}

private:
  
};

auto code_exception(std::string_view name, std::string_view desc, std::string_view src, Token token)
  -> Exception;

}  // namespace mcc

#endif
