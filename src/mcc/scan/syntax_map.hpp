#ifndef MCC_SYNTAX_MAP_HPP
#define MCC_SYNTAX_MAP_HPP

#include "regex/regex.hpp"
#include "trait.hpp"
#include <span>

namespace mcc {
using namespace trait;

using SyntaxMap = std::span<const std::pair<u32, Regex>>;

// TODO: implement trigraphs
// ??=      #
// ??(      [
// ??/      \
  // ??)      ]
// ??'      ^
// ??<      {
// ??!      |
// ??>      }
// ??-      ~

static auto syntax_ansi() -> SyntaxMap {
  static const std::pair<u32, Regex> map[]{
    {Blank, "{_|'@'}+"},
    {CommentSL, "'//' {{{'\\'^}|^} ~ /'\n'}? /'\n'"},
    {CommentML, "'/*' ^~ '*/'"},
    {BadComment, "'/*'"},
    {Directive, "'#' {{{'\\'^}|^} ~ /'\n'}? /'\n'"},

    {Sizeof, "'sizeof' /!a"},
    {Star, "'*'"},

    // Every token has the same regex pattern "'<name>' /!a"
    {KwAuto, "'auto' /!a"},
    {KwDouble, "'double' /!a"},
    {KwChar, "'char' /!a"},
    {KwFloat, "'float' /!a"},
    {KwInt, "'int' /!a"},
    {KwLong, "'long' /!a"},
    {KwShort, "'short' /!a"},
    {KwVoid, "'void' /!a"},
    {KwEnum, "'enum' /!a"},
    {KwTypedef, "'typedef' /!a"},
    {KwUnion, "'union' /!a"},
    {KwStruct, "'struct' /!a"},
    {KwVolatile, "'volatile' /!a"},
    {KwConst, "'const' /!a"},
    {KwExtern, "'extern' /!a"},
    {KwRegister, "'register' /!a"},
    {KwStatic, "'static' /!a"},
    {KwSigned, "'signed' /!a"},
    {KwUnsigned, "'unsigned' /!a"},
    {KwBreak, "'break' /!a"},
    {KwCase, "'case' /!a"},
    {KwContinue, "'continue' /!a"},
    {KwDefault, "'default' /!a"},
    {KwDo, "'do' /!a"},
    {KwElse, "'else' /!a"},
    {KwFor, "'for' /!a"},
    {KwGoto, "'goto' /!a"},
    {KwIf, "'if' /!a"},
    {KwReturn, "'return' /!a"},
    {KwSwitch, "'switch' /!a"},
    {KwWhile, "'while' /!a"},

    {CurlyBegin, "'{'"},
    {CurlyClose, "'}'"},
    {ParenBegin, "'('"},
    {ParenClose, "')'"},
    {CrochetBegin, "']'"},
    {CrochetClose, "'['"},

    {Arrow, "'->'"},
    {Increment, "'++'"},
    {Decrement, "'--'"},
    {Add, "'+'"},
    {Sub, "'-'"},

    {
      Float,
      "{[0-9]+ '.' [0-9]*} |"
      "{[0-9]* '.' [0-9]+}  "
      "{'e'|'E' {'+'|'-'}? [0-9]+}?"
      "a*",
    },

    {
      Integer,
      "{'0b'|'0B' [0-1]+                     } |"
      "{'0x'|'0X' [0-9]|[a-f]|[A-F]+         } |"
      "{[0-9]+ {'e'|'E' {'+'|'-'}? [0-9]+ }? }  "
      "a*",
    },

    {String, "'L'? Q {{{'\\'^}|^} ~ /{Q|'\n'}} ? {Q|'\n'}"},
    {BadString, "'L'? Q"},
    {Char, "'L'? q {{{'\\'^}|^} ~ /{q|'\n'}} {q|'\n'}"},
    {EmptyChar, "'L'? qq"},
    {BadChar, "'L'? q"},
    {Identifier, "{a|'_'} {a|'_'|n}*"},

    {Query, "'?'"},
    {Colon, "':'"},
    {Semicolon, "';'"},
    {Comma, "','"},
    {Dot, "'.'"},
    {And, "'&&'"},
    {Or, "'||'"},
    {BinNot, "'~'"},
    {BinOr, "'|'"},
    {BinXor, "'^'"},
    {BinShiftL, "'<<'"},
    {BinShiftR, "'>>'"},
    {Div, "'/'"},
    {Mod, "'%'"},
    {Equal, "'=='"},
    {NotEq, "'!='"},
    {LessEq, "'<='"},
    {GreaterEq, "'>='"},
    {Less, "'<'"},
    {Greater, "'>'"},
    {Not, "'!'"},
    {Assign, "'='"},
    {Ampersand, "'&'"},

    {None, "{^~/_}"},
  };

  return {map};
}

}  // namespace mcc

#endif
