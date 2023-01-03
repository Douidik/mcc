#ifndef MCC_TRAIT_HPP
#define MCC_TRAIT_HPP

#include "mcc.hpp"

// A trait should always be typed as u32 !
namespace mcc::trait {

// Trait Layout
// C: Class bit
// G: Group bit
// T: Type bit
// Each trait is 32 bits long {Class: 8 bits, Group: 11 bits, T: 13 bits}
// C|C|C|C|C|C|C|C|G|G|G|G|G|G|G|G|G|G|G|T|T|T|T|T|T|T|T|T|T|T|T|T

constexpr u32 TRAIT_SIZE = 32;

constexpr u32 CLASS_SIZE = 8;
constexpr u32 GROUP_SIZE = 11;
constexpr u32 TYPE_SIZE  = 13;

constexpr u32 CLASS_MASK = 0b1111'1111'0000'0000'0000'0000'0000'0000;
constexpr u32 GROUP_MASK = 0b0000'0000'1111'1111'1110'0000'0000'0000;
constexpr u32 TYPE_MASK  = 0b0000'0000'0000'0000'0001'1111'1111'1111;

static_assert(CLASS_SIZE + GROUP_SIZE + TYPE_SIZE == TRAIT_SIZE);

template<u32 N>
constexpr auto define_class() -> u32 {
  static_assert(N < CLASS_SIZE);
  return 1 << (TRAIT_SIZE - CLASS_SIZE + N);
}

template<u32 N>
constexpr auto define_group() -> u32 {
  static_assert(N < GROUP_SIZE);
  return 1 << (TRAIT_SIZE - CLASS_SIZE - GROUP_SIZE + N);
}

template<u32 C, u32 G, u32 T>
constexpr auto define_type() -> u32 {
  static_assert(T < TYPE_SIZE);
  return C | G | (1 << T);
}

enum TraitEnum : u32 {
  CsNone = 0,

  CsMeta       = define_class<0>(),
  CsKeyword    = define_class<1>(),
  CsIdentifier = define_class<2>(),
  CsConstant   = define_class<3>(),
  CsString     = define_class<4>(),
  CsOperator   = define_class<5>(),
  CsPunctuator = define_class<6>(),
  CsCatch      = define_class<7>(),

  GpNone       = 0,
  GpDefine     = define_group<0>(),
  GpFlow       = define_group<1>(),
  GpType       = define_group<2>(),
  GpModifier   = define_group<3>(),
  GpArithmetic = define_group<4>(),
  GpLogic      = define_group<5>(),
  GpCompare    = define_group<6>(),
  GpAccess     = define_group<7>(),
  GpBin        = define_group<8>(),
  GpBinaryOp   = define_group<9>(),
  GpBracket    = define_group<10>(),

  Blank     = define_type<CsMeta, GpNone, 1>(),
  End       = define_type<CsMeta, GpNone, 2>(),
  CommentSL = define_type<CsMeta, GpNone, 3>(),
  CommentML = define_type<CsMeta, GpNone, 4>(),
  Directive = define_type<CsMeta, GpNone, 5>(),

  Sizeof    = define_type<CsKeyword | CsOperator, GpAccess, 0>(),
  Ampersand = define_type<CsOperator, GpAccess | GpBin | GpBinaryOp, 0>(),
  Star =
    define_type<CsPunctuator | CsKeyword, GpAccess | GpModifier | GpArithmetic | GpBinaryOp, 0>(),

  KwAuto     = define_type<CsKeyword, GpType, 0>(),
  KwDouble   = define_type<CsKeyword, GpType, 1>(),
  KwChar     = define_type<CsKeyword, GpType, 2>(),
  KwFloat    = define_type<CsKeyword, GpType, 3>(),
  KwInt      = define_type<CsKeyword, GpType, 4>(),
  KwVoid     = define_type<CsKeyword, GpType, 5>(),
  KwLong     = define_type<CsKeyword, GpModifier, 0>(),
  KwShort    = define_type<CsKeyword, GpModifier, 1>(),
  KwVolatile = define_type<CsKeyword, GpModifier, 2>(),
  KwConst    = define_type<CsKeyword, GpModifier, 3>(),
  KwExtern   = define_type<CsKeyword, GpModifier, 4>(),
  KwRegister = define_type<CsKeyword, GpModifier, 5>(),
  KwStatic   = define_type<CsKeyword, GpModifier, 6>(),
  KwSigned   = define_type<CsKeyword, GpModifier, 7>(),
  KwUnsigned = define_type<CsKeyword, GpModifier, 8>(),
  KwEnum     = define_type<CsKeyword, GpDefine, 0>(),
  KwTypedef  = define_type<CsKeyword, GpDefine, 1>(),
  KwUnion    = define_type<CsKeyword, GpDefine, 2>(),
  KwStruct   = define_type<CsKeyword, GpDefine, 3>(),
  KwPointer  = Star,
  KwBreak    = define_type<CsKeyword, GpFlow, 0>(),
  KwCase     = define_type<CsKeyword, GpFlow, 1>(),
  KwContinue = define_type<CsKeyword, GpFlow, 2>(),
  KwDefault  = define_type<CsKeyword, GpFlow, 3>(),
  KwDo       = define_type<CsKeyword, GpFlow, 4>(),
  KwElse     = define_type<CsKeyword, GpFlow, 5>(),
  KwFor      = define_type<CsKeyword, GpFlow, 6>(),
  KwGoto     = define_type<CsKeyword, GpFlow, 7>(),
  KwIf       = define_type<CsKeyword, GpFlow, 8>(),
  KwReturn   = define_type<CsKeyword, GpFlow, 9>(),
  KwSwitch   = define_type<CsKeyword, GpFlow, 10>(),
  KwWhile    = define_type<CsKeyword, GpFlow, 11>(),

  Identifier = define_type<CsIdentifier, GpNone, 0>(),

  Float   = define_type<CsConstant, GpNone, 0>(),
  Integer = define_type<CsConstant, GpNone, 1>(),
  String  = define_type<CsConstant, GpNone, 2>(),
  Char    = define_type<CsConstant, GpNone, 3>(),

  Query        = define_type<CsOperator, GpFlow, 0>(),
  Increment    = define_type<CsOperator, GpNone, 0>(),
  Decrement    = define_type<CsOperator, GpNone, 1>(),
  CurlyBegin   = define_type<CsOperator, GpBracket, 0>(),
  CurlyClose   = define_type<CsOperator, GpBracket, 1>(),
  ParenBegin   = define_type<CsOperator, GpBracket, 2>(),
  ParenClose   = define_type<CsOperator, GpBracket, 3>(),
  CrochetBegin = define_type<CsOperator, GpBracket, 4>(),
  CrochetClose = define_type<CsOperator, GpBracket, 5>(),
  Assign       = define_type<CsOperator, GpBinaryOp, 0>(),
  Not          = define_type<CsOperator, GpLogic, 0>(),
  And          = define_type<CsOperator, GpLogic | GpBinaryOp, 0>(),
  Or           = define_type<CsOperator, GpLogic | GpBinaryOp, 1>(),
  Add          = define_type<CsOperator, GpArithmetic | GpBinaryOp, 0>(),
  Sub          = define_type<CsOperator, GpArithmetic | GpBinaryOp, 1>(),
  Mul          = Star,
  Div          = define_type<CsOperator, GpArithmetic | GpBinaryOp, 2>(),
  Mod          = define_type<CsOperator, GpArithmetic | GpBinaryOp, 3>(),
  BinNot       = define_type<CsOperator, GpBin, 0>(),
  BinAnd       = Ampersand,
  BinOr        = define_type<CsOperator, GpBin | GpBinaryOp, 0>(),
  BinXor       = define_type<CsOperator, GpBin | GpBinaryOp, 1>(),
  BinShiftL    = define_type<CsOperator, GpBin | GpBinaryOp, 2>(),
  BinShiftR    = define_type<CsOperator, GpBin | GpBinaryOp, 3>(),
  Equal        = define_type<CsOperator, GpCompare | GpBinaryOp, 0>(),
  NotEq        = define_type<CsOperator, GpCompare | GpBinaryOp, 1>(),
  Less         = define_type<CsOperator, GpCompare | GpBinaryOp, 2>(),
  Greater      = define_type<CsOperator, GpCompare | GpBinaryOp, 3>(),
  LessEq       = define_type<CsOperator, GpCompare | GpBinaryOp, 4>(),
  GreaterEq    = define_type<CsOperator, GpCompare | GpBinaryOp, 5>(),
  Deref        = Star,
  Address      = Ampersand,
  Dot          = define_type<CsOperator, GpAccess, 1>(),
  Arrow        = define_type<CsOperator, GpAccess, 2>(),
  Comma        = define_type<CsPunctuator, GpNone, 1>(),
  Colon        = define_type<CsPunctuator, GpNone, 2>(),
  Semicolon    = define_type<CsPunctuator, GpNone, 3>(),

  None       = define_type<CsMeta | CsCatch, GpNone, 0>(),
  BadComment = define_type<CsMeta | CsCatch, None, 1>(),
  BadString  = define_type<CsMeta | CsCatch, CsConstant, 0>(),
  BadChar    = define_type<CsMeta | CsCatch, CsConstant, 1>(),
  EmptyChar  = define_type<CsMeta | CsCatch, CsConstant, 2>(),
};

}  // namespace mcc::trait

namespace mcc {
using namespace trait;

constexpr auto trait_class(u32 trait) -> u32 {
  return (trait & CLASS_MASK) >> GROUP_SIZE >> TYPE_SIZE;
}

constexpr auto trait_group(u32 trait) -> u32 {
  return (trait & GROUP_MASK) >> TYPE_SIZE;
}

constexpr auto trait_type(u32 trait) -> u32 {
  return (trait & TYPE_MASK);
}

constexpr auto trait_decompose(u32 trait) -> std::tuple<u32, u32, u32> {
  return {trait_class(trait), trait_group(trait), trait_type(trait)};
}

constexpr auto trait_class_desc(u32 group) -> std::string_view {
  switch (group & CLASS_MASK) {
  case CsMeta: return "Class-Meta";
  case CsKeyword: return "Class-Keyword";
  case CsIdentifier: return "Class-Identifier";
  case CsConstant: return "Class-Constant";
  case CsString: return "Class-String";
  case CsOperator: return "Class-Operator";
  case CsPunctuator: return "Class-Punctuator";
  case CsCatch: return "Class-Catch";
  default: return "?";
  }
}

constexpr auto trait_group_desc(u32 group) -> std::string_view {
  switch (group & GROUP_MASK) {
  case GpNone: return "Group-None";
  case GpDefine: return "Group-Define";
  case GpFlow: return "Group-Flow";
  case GpType: return "Group-Type";
  case GpModifier: return "Group-Modifier";
  case GpArithmetic: return "Group-Arithmetic";
  case GpLogic: return "Group-Logic";
  case GpCompare: return "Group-Compare";
  case GpAccess: return "Group-Access";
  case GpBin: return "Group-Bin";
  case GpBinaryOp: return "Group-BinaryOp";
  case GpBracket: return "Group-Bracket";
  default: return "?";
  }
}

constexpr auto trait_type_desc(u32 trait) -> std::string_view {
  switch (trait) {
  case Blank: return "Blank";
  case End: return "End";
  case CommentSL: return "Comment-SL";
  case CommentML: return "Comment-ML";
  case Directive: return "Directive";
  case Sizeof: return "sizeof";
  case Star: return "*";
  case Ampersand: return "&";
  case KwAuto: return "auto";
  case KwDouble: return "double";
  case KwChar: return "char";
  case KwFloat: return "float";
  case KwInt: return "int";
  case KwLong: return "long";
  case KwShort: return "short";
  case KwVoid: return "void";
  case KwEnum: return "enum";
  case KwTypedef: return "typedef";
  case KwUnion: return "union";
  case KwStruct: return "struct";
  case KwVolatile: return "volatile";
  case KwConst: return "const";
  case KwExtern: return "extern";
  case KwRegister: return "KwRegister";
  case KwStatic: return "static";
  case KwSigned: return "signed";
  case KwUnsigned: return "unsigned";
  case KwBreak: return "break";
  case KwCase: return "case";
  case KwContinue: return "continue";
  case KwDefault: return "default";
  case KwDo: return "do";
  case KwElse: return "else";
  case KwFor: return "for";
  case KwGoto: return "goto";
  case KwIf: return "if";
  case KwReturn: return "return";
  case KwSwitch: return "switch";
  case KwWhile: return "while";
  case CurlyBegin: return "{";
  case CurlyClose: return "}";
  case ParenBegin: return "(";
  case ParenClose: return ")";
  case CrochetBegin: return "[";
  case CrochetClose: return "]";
  case Identifier: return "Identifier";
  case Float: return "Float";
  case Integer: return "Integer";
  case String: return "String";
  case Char: return "Char";
  case Increment: return "++";
  case Decrement: return "--";
  case Assign: return "=";
  case Not: return "!";
  case And: return "&&";
  case Or: return "||";
  case Add: return "+";
  case Sub: return "-";
  case Div: return "/";
  case Mod: return "%";
  case BinNot: return "~";
  case BinOr: return "|";
  case BinXor: return "^";
  case BinShiftL: return "<<";
  case BinShiftR: return ">>";
  case Equal: return "==";
  case NotEq: return "!=";
  case Less: return "<";
  case Greater: return ">";
  case LessEq: return "<=";
  case GreaterEq: return ">=";
  case Dot: return ".";
  case Arrow: return "->";
  case Comma: return ",";
  case Colon: return ":";
  case Semicolon: return ";";
  case None: return "None";
  case BadComment: return "BadComment";
  case BadString: return "BadString";
  case BadChar: return "BadChar";
  case EmptyChar: return "EmptyChar";
  }

  return "?";
}

constexpr auto trait_catch_desc(u32 trait) -> std::string_view {
  switch (trait) {
  case None: return "Token not recognized";
  case BadComment: return "Umatched comment delimiter, missing <*/> ending delimiter";
  case BadString: return "Umatched string delimiter, missing <\"> ending delimiter";
  case BadChar: return "Umatched character delimiter, missing <'> ending delimiter";
  case EmptyChar: return "Empty character constant";
  }

  return "?";
}
}  // namespace mcc

#endif
