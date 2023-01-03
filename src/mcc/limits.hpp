#ifndef MCC_LIMITS_HPP
#define MCC_LIMITS_HPP

#include "mcc.hpp"

namespace mcc::max {
/*
  From: 'http://port70.net/~nsz/c/c89/c89-draft.html#3.1.3.1'
  2.2.4.1 Translation limits
  The implementation shall be able to translate and execute at least one program that contains at least one instance of every one of the following limits:6
* 15 nesting levels of compound statements, iteration control structures, and selection control structures
* 8 nesting levels of conditional inclusion
* 12 pointer, array, and function declarators (in any combinations) modifying an arithmetic, a structure, a union, or an incomplete type in a declaration
* 31 declarators nested by parentheses within a full declarator
* 32 expressions nested by parentheses within a full expression
* 31 significant initial characters in an internal identifier or a macro name
* 6 significant initial characters in an external identifier
* 511 external identifiers in one translation unit
* 127 identifiers with block scope declared in one block
* 1024 macro identifiers simultaneously defined in one translation unit
* 31 parameters in one function definition
* 31 arguments in one function call
* 31 parameters in one macro definition
* 31 arguments in one macro invocation
* 509 characters in a logical source line
* 509 characters in a character string literal or wide string literal (after concatenation)
* 32767 bytes in an object (in a hosted environment only)
* 8 nesting levels for #include'd files
* 257 case labels for a switch statement (excluding those for any nested switch statements)
* 127 members in a single structure or union
* 127 enumeration constants in a single enumeration
* 15 levels of nested structure or union definitions in a single struct-declaration-list
 */

//TODO: Define all the C standard limitations
constexpr u32 FUNC_ARGS = 31;
constexpr u32 EXPR_NESTED = 32;
constexpr u32 ENUM_DECLS = 127;
constexpr u32 STRUCT_MEMBERS = 127;

// constexpr u32 COMPOUNDS_NESTING = 15;
// constexpr u32 COND_NESTING = 8;
// constexpr u32 DECL_MAX_MODIFY = 12;
// constexpr u32 DECL_PAREN_NESTING = 31;
// constexpr u32  = 8;
// constexpr u32 CONDITIONAL_NESTING = 8;
// constexpr u32 CONDITIONAL_NESTING = 8;
// constexpr u32 CONDITIONAL_NESTING = 8;
// constexpr u32 CONDITIONAL_NESTING = 8;
// constexpr u32 CONDITIONAL_NESTING = 8;

}  // namespace mcc::max

#endif
