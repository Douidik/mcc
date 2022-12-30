#ifndef MCC_TRAIT_HPP
#define MCC_TRAIT_HPP

#include "mcc.hpp"

// A trait should always be typed as u32 !
namespace mcc::trait {

// Trait Layout
// C: Class bit
// G: Group bit
// N: Disriminant bit
// Each trait is 32 bits long {Class: 8 bits, Group: 11 bits, N: 13 bits}
// C|C|C|C|C|C|C|C|G|G|G|G|G|G|G|G|G|G|G|N|N|N|N|N|N|N|N|N|N|N|N|N

constexpr u32 TRAIT_SIZE = 32;
constexpr u32 CLASS_SIZE = 8;
constexpr u32 GROUP_SIZE = 11;
constexpr u32 N_SIZE = 13;

static_assert(CLASS_SIZE + GROUP_SIZE + N_SIZE == TRAIT_SIZE);

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

template<u32 C, u32 G, u32 N>
constexpr auto define() -> u32 {
  return N | C | G;
}

enum TraitEnum : u32 {
  CsNone = 0,

  CsMeta = define_class<0>(),
  CsKeyword = define_class<1>(),
  CsIdentifier = define_class<2>(),
  CsConstant = define_class<3>(),
  CsString = define_class<4>(),
  CsOperator = define_class<5>(),
  CsPunctuator = define_class<6>(),
  CsCatch = define_class<7>(),

  GpNone = 0,
  GpDefine = define_group<0>(),
  GpFlow = define_group<1>(),
  GpType = define_group<2>(),
  GpModifier = define_group<3>(),
  GpArithmetic = define_group<4>(),
  GpLogic = define_group<5>(),
  GpCompare = define_group<6>(),
  GpAccess = define_group<7>(),
  GpBin = define_group<8>(),
  GpBinaryOp = define_group<9>(),
  GpBracket = define_group<10>(),

  Blank = define<CsMeta, GpNone, 1>(),
  End = define<CsMeta, GpNone, 2>(),
  CommentSL = define<CsMeta, GpNone, 3>(),
  CommentML = define<CsMeta, GpNone, 4>(),
  Directive = define<CsMeta, GpNone, 5>(),

  Sizeof = define<CsKeyword | CsOperator, GpAccess, 0>(),
  Star = define<CsPunctuator | CsKeyword, GpAccess | GpModifier | GpArithmetic | GpBinaryOp, 0>(),
  Ampersand = define<CsOperator, GpAccess | GpBin | GpBinaryOp, 0>(),

  KwAuto = define<CsKeyword, GpType, 0>(),
  KwDouble = define<CsKeyword, GpType, 1>(),
  KwChar = define<CsKeyword, GpType, 2>(),
  KwFloat = define<CsKeyword, GpType, 3>(),
  KwInt = define<CsKeyword, GpType, 4>(),
  KwVoid = define<CsKeyword, GpType, 5>(),
  KwLong = define<CsKeyword, GpType | GpModifier, 0>(),
  KwShort = define<CsKeyword, GpType | GpModifier, 1>(),
  KwEnum = define<CsKeyword, GpDefine, 0>(),
  KwTypedef = define<CsKeyword, GpDefine, 1>(),
  KwUnion = define<CsKeyword, GpDefine, 2>(),
  KwStruct = define<CsKeyword, GpDefine, 3>(),
  KwVolatile = define<CsKeyword, GpModifier, 0>(),
  KwConst = define<CsKeyword, GpModifier, 1>(),
  KwExtern = define<CsKeyword, GpModifier, 2>(),
  KwRegister = define<CsKeyword, GpModifier, 3>(),
  KwStatic = define<CsKeyword, GpModifier, 4>(),
  KwSigned = define<CsKeyword, GpModifier, 5>(),
  KwUnsigned = define<CsKeyword, GpModifier, 6>(),
  KwPointer = Star,
  KwBreak = define<CsKeyword, GpFlow, 0>(),
  KwCase = define<CsKeyword, GpFlow, 1>(),
  KwContinue = define<CsKeyword, GpFlow, 2>(),
  KwDefault = define<CsKeyword, GpFlow, 3>(),
  KwDo = define<CsKeyword, GpFlow, 4>(),
  KwElse = define<CsKeyword, GpFlow, 5>(),
  KwFor = define<CsKeyword, GpFlow, 6>(),
  KwGoto = define<CsKeyword, GpFlow, 7>(),
  KwIf = define<CsKeyword, GpFlow, 8>(),
  KwReturn = define<CsKeyword, GpFlow, 9>(),
  KwSwitch = define<CsKeyword, GpFlow, 10>(),
  KwWhile = define<CsKeyword, GpFlow, 11>(),

  Identifier = define<CsIdentifier, GpNone, 0>(),

  Float = define<CsConstant, GpNone, 0>(),
  FloatBin = define<CsConstant, GpNone, 0>(),
  FloatHex = define<CsConstant, GpNone, 0>(),
  Integer = define<CsConstant, GpNone, 1>(),
  String = define<CsConstant, GpNone, 2>(),
  Char = define<CsConstant, GpNone, 3>(),

  Query = define<CsOperator, GpFlow, 0>(),
  Increment = define<CsOperator, GpNone, 0>(),
  Decrement = define<CsOperator, GpNone, 1>(),
  CurlyBegin = define<CsOperator, GpBracket, 0>(),
  CurlyClose = define<CsOperator, GpBracket, 1>(),
  ParenBegin = define<CsOperator, GpBracket, 2>(),
  ParenClose = define<CsOperator, GpBracket, 3>(),
  CrochetBegin = define<CsOperator, GpBracket, 4>(),
  CrochetClose = define<CsOperator, GpBracket, 5>(),
  Assign = define<CsOperator, GpBinaryOp, 0>(),
  Not = define<CsOperator, GpLogic, 0>(),
  And = define<CsOperator, GpLogic | GpBinaryOp, 0>(),
  Or = define<CsOperator, GpLogic | GpBinaryOp, 1>(),
  Add = define<CsOperator, GpArithmetic | GpBinaryOp, 0>(),
  Sub = define<CsOperator, GpArithmetic | GpBinaryOp, 1>(),
  Mul = Star,
  Div = define<CsOperator, GpArithmetic | GpBinaryOp, 2>(),
  Mod = define<CsOperator, GpArithmetic | GpBinaryOp, 3>(),
  BinNot = define<CsOperator, GpBin, 0>(),
  BinAnd = Ampersand,
  BinOr = define<CsOperator, GpBin | GpBinaryOp, 0>(),
  BinXor = define<CsOperator, GpBin | GpBinaryOp, 1>(),
  BinShiftL = define<CsOperator, GpBin | GpBinaryOp, 2>(),
  BinShiftR = define<CsOperator, GpBin | GpBinaryOp, 3>(),
  Equal = define<CsOperator, GpCompare | GpBinaryOp, 0>(),
  NotEq = define<CsOperator, GpCompare | GpBinaryOp, 1>(),
  Less = define<CsOperator, GpCompare | GpBinaryOp, 2>(),
  Greater = define<CsOperator, GpCompare | GpBinaryOp, 3>(),
  LessEq = define<CsOperator, GpCompare | GpBinaryOp, 4>(),
  GreaterEq = define<CsOperator, GpCompare | GpBinaryOp, 5>(),
  Deref = Star,
  Address = Ampersand,
  Dot = define<CsOperator, GpAccess, 1>(),
  Arrow = define<CsOperator, GpAccess, 2>(),
  Comma = define<CsPunctuator, GpNone, 1>(),
  Colon = define<CsPunctuator, GpNone, 2>(),
  Semicolon = define<CsPunctuator, GpNone, 3>(),

  None = define<CsMeta | CsCatch, GpNone, 0>(),
  BadComment = define<CsMeta | CsCatch, None, 1>(),
  BadString = define<CsMeta | CsCatch, CsConstant, 0>(),
  BadChar = define<CsMeta | CsCatch, CsConstant, 1>(),
  EmptyChar = define<CsMeta | CsCatch, CsConstant, 2>(),
};

}  // namespace mcc::trait

namespace mcc {
using namespace trait;

constexpr auto trait_name(u32 trait) -> std::string_view {
  switch (trait) {
  case Blank: return "Blank";
  case End: return "End";
  case CommentSL: return "CommentSL";
  case CommentML: return "CommentML";
  case Directive: return "Directive";
  case Sizeof: return "Sizeof";
  case Star: return "Star";
  case Ampersand: return "Ampersand";
  case KwAuto: return "KwAuto";
  case KwDouble: return "KwDouble";
  case KwChar: return "KwChar";
  case KwFloat: return "KwFloat";
  case KwInt: return "KwInt";
  case KwLong: return "KwLong";
  case KwShort: return "KwShort";
  case KwVoid: return "KwVoid";
  case KwEnum: return "KwEnum";
  case KwTypedef: return "KwTypedef";
  case KwUnion: return "KwUnion";
  case KwStruct: return "KwStruct";
  case KwVolatile: return "KwVolatile";
  case KwConst: return "KwConst";
  case KwExtern: return "KwExtern";
  case KwRegister: return "KwRegister";
  case KwStatic: return "KwStatic";
  case KwSigned: return "KwSigned";
  case KwUnsigned: return "KwUnsigned";
  case KwBreak: return "KwBreak";
  case KwCase: return "KwCase";
  case KwContinue: return "KwContinue";
  case KwDefault: return "KwDefault";
  case KwDo: return "KwDo";
  case KwElse: return "KwElse";
  case KwFor: return "KwFor";
  case KwGoto: return "KwGoto";
  case KwIf: return "KwIf";
  case KwReturn: return "KwReturn";
  case KwSwitch: return "KwSwitch";
  case KwWhile: return "KwWhile";
  case CurlyBegin: return "CurlyBegin";
  case CurlyClose: return "CurlyClose";
  case ParenBegin: return "ParenBegin";
  case ParenClose: return "ParenClose";
  case CrochetBegin: return "CrochetBegin";
  case CrochetClose: return "CrochetClose";
  case Identifier: return "Identifier";
  case Float: return "Float";
  case Integer: return "Integer";
  case String: return "String";
  case Char: return "Char";
  case Increment: return "Increment";
  case Decrement: return "Decrement";
  case Assign: return "Assign";
  case Not: return "Not";
  case And: return "And";
  case Or: return "Or";
  case Add: return "Add";
  case Sub: return "Sub";
  case Div: return "Div";
  case Mod: return "Mod";
  case BinNot: return "BinNot";
  case BinOr: return "BinOr";
  case BinXor: return "BinXor";
  case BinShiftL: return "BinShiftL";
  case BinShiftR: return "BinShiftR";
  case Equal: return "Equal";
  case NotEq: return "NotEq";
  case Less: return "Less";
  case Greater: return "Greater";
  case LessEq: return "LessEq";
  case GreaterEq: return "GreaterEq";
  case Dot: return "Dot";
  case Arrow: return "Arrow";
  case Comma: return "Comma";
  case Colon: return "Colon";
  case Semicolon: return "Semicolon";
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
