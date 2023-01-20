#ifndef MCC_LEXER_TEST_HPP
#define MCC_LEXER_TEST_HPP

#include "scan/lexer.hpp"
#include <gtest/gtest.h>

namespace mcc {
using namespace trait;

static auto match_tokens(std::string_view src, const std::vector<Token> &&tokens)
  -> testing::AssertionResult {
  std::string endlined_source = fmt::format("{}\n", src);
  Lexer lexer{endlined_source};
  for (const Token &expected : tokens) {
    auto token = lexer.tokenize();

    if (token.trait != expected.trait) {
      auto token_trait = trait_type_desc(token.trait);
      auto expected_trait = trait_type_desc(expected.trait);
      return testing::AssertionFailure() << token_trait << " != " << expected_trait;
    }

    if (!(token.trait & CsKeyword) && token.src != expected.src) {
      return testing::AssertionFailure() << "'" << token.src << "' != '" << expected.src << "'";
    }
  }

  return testing::AssertionSuccess();
}

#define EXPECT_TOKENS(source, ...) EXPECT_TRUE(match_tokens(source, {__VA_ARGS__}))

#define EXPECT_LEXER_EXCEPTION(source) \
  EXPECT_THROW( \
    { \
      Lexer lexer{source}; \
      for (;;) { \
        Token token = lexer.tokenize(); \
        if (token.trait == End) break; \
      } \
    }, \
    Exception)

// Syntax test, we try to initialize the syntax map catching regex patterns errors

TEST(Lexer, SyntaxAnsi) {
  auto syntax = syntax_ansi();
}

TEST(Lexer, Comment) {
  EXPECT_TOKENS("// Hello World \n", {"// Hello World ", CommentSL});

  EXPECT_TOKENS(
    R"(
// This is a comment 
int main() {
// A second one //
)",
    {"// This is a comment ", CommentSL},
    {"int", KwInt},
    {"main", Identifier},
    {"(", ParenBegin},
    {")", ParenClose},
    {"{", CurlyBegin},
    {"// A second one //", CommentSL});

  EXPECT_TOKENS("/* Hello World */\n", {"/* Hello World */", CommentML});

  EXPECT_TOKENS(
    R"(
/*
 * main() -> int
 * Arguments: None
 * Return: Program execution status, if main() != 0, then the program execution failed
 * This function is the program starting point of the program, must be declared in any program
*/
int main() {
/* This is an inline comment, that is delimited by :*/
)",

    {R"(/*
 * main() -> int
 * Arguments: None
 * Return: Program execution status, if main() != 0, then the program execution failed
 * This function is the program starting point of the program, must be declared in any program
*/)",
     CommentML},
    {"int", KwInt},
    {"main", Identifier},
    {"(", ParenBegin},
    {")", ParenClose},
    {"{", CurlyBegin},
    {"/* This is an inline comment, that is delimited by :*/", CommentML});

  EXPECT_TOKENS(
    R"(
// Mutiline single-line comments	\
yes for some reason that's a thing\
it's very embarrassing            \

auto main() -> Integer { return EXIT_SUCCESS; }
)",
    {
      R"(// Mutiline single-line comments	\
yes for some reason that's a thing\
it's very embarrassing            \
)",
      CommentSL});

  // NOTE: Seems the compiler is not interpreting well \ at the end of raw-string well
  // we can't test malformed escaped single line comments
  //   EXPECT_TOKENS(
  //     R"(
  // // This multiline comment is malformed \<Should be spaces !!!>
  // ninja spaces are preventing me to be a comment, just a bunch of identifiers
  // )",
  //     {R"(// This multiline comment is malformed \ )", CommentSL},
  //     {"ninja", Identifier},
  //     {"spaces", Identifier},
  //     {"are", Identifier},
  //     {"preventing", Identifier},
  //     {"me", Identifier},
  //     {"to", Identifier},
  //     {"be", Identifier},
  //     {"a", Identifier},
  //     {"comment", Identifier},
  //     {",", Comma},
  //     {"just", Identifier},
  //     {"a", Identifier},
  //     {"bunch", Identifier},
  //     {"of", Identifier},
  //     {"identifiers", Identifier});

  EXPECT_LEXER_EXCEPTION("/* This comment should be terminated !!! \n");

  EXPECT_LEXER_EXCEPTION(
    R"(/* 
This
comment
should
be
terminated
!!!
)");
}

TEST(Lexer, Directive) {
  EXPECT_TOKENS("#define TRUE (1)\n", {"#define TRUE (1)", Directive});
  EXPECT_TOKENS("#define FALSE (0)\n", {"#define FALSE (0)", Directive});

  EXPECT_TOKENS(
    R"(
#define POW(x, n) do { for (int i = 0; i < n; i++) { \
  x = x * x;                                         \
} while (0)

int main() {}
)",
    {
      R"(#define POW(x, n) do { for (int i = 0; i < n; i++) { \
  x = x * x;                                         \
} while (0))",
      Directive,
    });

  EXPECT_TOKENS("#  include <stdio.h>\n", {"#  include <stdio.h>", Directive});

  EXPECT_TOKENS(
    R"(
#ifndef CCC_EXAMPLE_H
#  define CCC_EXAMPLE_H

#  define CCC_VAL \
  (-1)  

#endif
)",
    {"#ifndef CCC_EXAMPLE_H", Directive},
    {"#  define CCC_EXAMPLE_H", Directive},
    {"#  define CCC_VAL \\\n  (-1)  ", Directive},
    {"#endif", Directive});
}

TEST(Lexer, Keyword) {
  EXPECT_TOKENS("auto", {"auto", KwAuto});
  EXPECT_TOKENS("double", {"double", KwDouble});
  EXPECT_TOKENS("char", {"char", KwChar});
  EXPECT_TOKENS("float", {"float", KwFloat});
  EXPECT_TOKENS("int", {"int", KwInt});
  EXPECT_TOKENS("void", {"void", KwVoid});
  EXPECT_TOKENS("long", {"long", KwLong});
  EXPECT_TOKENS("short", {"short", KwShort});
  EXPECT_TOKENS("enum", {"enum", KwEnum});
  EXPECT_TOKENS("typedef", {"typedef", KwTypedef});
  EXPECT_TOKENS("union", {"union", KwUnion});
  EXPECT_TOKENS("struct", {"struct", KwStruct});
  EXPECT_TOKENS("volatile", {"volatile", KwVolatile});
  EXPECT_TOKENS("const", {"const", KwConst});
  EXPECT_TOKENS("extern", {"extern", KwExtern});
  EXPECT_TOKENS("register", {"register", KwRegister});
  EXPECT_TOKENS("static", {"static", KwStatic});
  EXPECT_TOKENS("signed", {"signed", KwSigned});
  EXPECT_TOKENS("unsigned", {"unsigned", KwUnsigned});
  EXPECT_TOKENS("*", {"*", KwPointer});
  EXPECT_TOKENS("break", {"break", KwBreak});
  EXPECT_TOKENS("case", {"case", KwCase});
  EXPECT_TOKENS("continue", {"continue", KwContinue});
  EXPECT_TOKENS("default", {"default", KwDefault});
  EXPECT_TOKENS("do", {"do", KwDo});
  EXPECT_TOKENS("else", {"else", KwElse});
  EXPECT_TOKENS("for", {"for", KwFor});
  EXPECT_TOKENS("goto", {"goto", KwGoto});
  EXPECT_TOKENS("if", {"if", KwIf});
  EXPECT_TOKENS("return", {"return", KwReturn});
  EXPECT_TOKENS("switch", {"switch", KwSwitch});
  EXPECT_TOKENS("while", {"while", KwWhile});
  EXPECT_TOKENS("sizeof", {"sizeof", Sizeof});

  EXPECT_TOKENS("        signed", {"signed", KwSigned});
  EXPECT_TOKENS("signed        ", {"signed", KwSigned});
}

TEST(Lexer, Operator) {
  EXPECT_TOKENS("++", {"++", Increment});
  EXPECT_TOKENS("--", {"--", Decrement});
  EXPECT_TOKENS("{", {"{", CurlyBegin});
  EXPECT_TOKENS("}", {"}", CurlyClose});
  EXPECT_TOKENS("(", {"(", ParenBegin});
  EXPECT_TOKENS(")", {")", ParenClose});
  EXPECT_TOKENS("]", {"]", CrochetBegin});
  EXPECT_TOKENS("[", {"[", CrochetClose});
  EXPECT_TOKENS(":", {":", Colon});
  EXPECT_TOKENS(";", {";", Semicolon});
  EXPECT_TOKENS(",", {",", Comma});
  EXPECT_TOKENS("&&", {"&&", And});
  EXPECT_TOKENS("||", {"||", Or});
  EXPECT_TOKENS(".", {".", Dot});
  EXPECT_TOKENS("->", {"->", Arrow});
  EXPECT_TOKENS("~", {"~", BinNot});
  EXPECT_TOKENS("|", {"|", BinOr});
  EXPECT_TOKENS("^", {"^", BinXor});
  EXPECT_TOKENS("<<", {"<<", BinShiftL});
  EXPECT_TOKENS(">>", {">>", BinShiftR});
  EXPECT_TOKENS("+", {"+", Add});
  EXPECT_TOKENS("-", {"-", Sub});
  EXPECT_TOKENS("/", {"/", Div});
  EXPECT_TOKENS("%", {"%", Mod});
  EXPECT_TOKENS("==", {"==", Equal});
  EXPECT_TOKENS("!=", {"!=", NotEq});
  EXPECT_TOKENS("<=", {"<=", LessEq});
  EXPECT_TOKENS(">=", {">=", GreaterEq});
  EXPECT_TOKENS("<", {"<", Less});
  EXPECT_TOKENS(">", {">", Greater});
  EXPECT_TOKENS("!", {"!", Not});
  EXPECT_TOKENS("=", {"=", Assign});
  EXPECT_TOKENS("&", {"&", Ampersand});

  // Operators doesn't require spaces to match
  EXPECT_TOKENS("!finished", {"!", Not}, {"finished", Identifier});
  EXPECT_TOKENS("! finished", {"!", Not}, {"finished", Identifier});

  // (Add/Sub) to (Increment/Decrement) Ambiguity
  EXPECT_TOKENS("i++", {"i", Identifier}, {"++", Increment});
  EXPECT_TOKENS("++i", {"++", Increment}, {"i", Identifier});
  EXPECT_TOKENS("i ++", {"i", Identifier}, {"++", Increment});
  EXPECT_TOKENS("++ i", {"++", Increment}, {"i", Identifier});
  EXPECT_TOKENS("+ +", {"+", Add}, {"+", Add});

  EXPECT_TOKENS("vec.x", {"vec", Identifier}, {".", Dot}, {"x", Identifier});
  EXPECT_TOKENS("pvec->size", {"pvec", Identifier}, {"->", Arrow}, {"size", Identifier});
}

TEST(Lexer, Integer) {
  EXPECT_TOKENS("1234567890", {"1234567890", Integer});
  EXPECT_TOKENS("0", {"0", Integer});
  EXPECT_TOKENS("-0", {"-", Sub}, {"0", Integer});
  EXPECT_TOKENS("+0", {"+", Add}, {"0", Integer});
  EXPECT_TOKENS("+2147483647", {"+", Add}, {"2147483647", Integer});
  EXPECT_TOKENS("-2147483648", {"-", Sub}, {"2147483648", Integer});
  EXPECT_TOKENS("-+0", {"-", Sub}, {"+", Add}, {"0", Integer});
  EXPECT_TOKENS("+-+0", {"+", Add}, {"-", Sub}, {"+", Add}, {"0", Integer});
  EXPECT_TOKENS("++0", {"++", Increment}, {"0", Integer});

  // A prefix without a becomes a suffix :-|
  EXPECT_TOKENS("0b", {"0b", Integer});
  EXPECT_TOKENS("0x", {"0x", Integer});

  EXPECT_TOKENS("0b0", {"0b0", Integer});
  EXPECT_TOKENS("0B1", {"0B1", Integer});
  EXPECT_TOKENS("0b1011101", {"0b1011101", Integer});
  EXPECT_TOKENS("0B1011101", {"0B1011101", Integer});

  EXPECT_TOKENS("0x0", {"0x0", Integer});
  EXPECT_TOKENS("0X0", {"0X0", Integer});
  EXPECT_TOKENS("0xF", {"0xF", Integer});
  EXPECT_TOKENS("0XF", {"0XF", Integer});
  EXPECT_TOKENS("0xf", {"0xf", Integer});
  EXPECT_TOKENS("0xF", {"0xF", Integer});
  EXPECT_TOKENS("0xaBcDeF0123456789", {"0xaBcDeF0123456789", Integer});
  EXPECT_TOKENS("+0xaBcDeF0123456789", {"+", Add}, {"0xaBcDeF0123456789", Integer});
  EXPECT_TOKENS("-0xaBcDeF0123456789", {"-", Sub}, {"0xaBcDeF0123456789", Integer});
  EXPECT_TOKENS("-+0xAbCdEf0123456789", {"-", Sub}, {"+", Add}, {"0xAbCdEf0123456789", Integer});
  EXPECT_TOKENS("+-0XaBcdEf0123456789", {"+", Add}, {"-", Sub}, {"0XaBcdEf0123456789", Integer});

  EXPECT_TOKENS("1234567890e0123456789", {"1234567890e0123456789", Integer});
  EXPECT_TOKENS("1234567890e+0123456789", {"1234567890e+0123456789", Integer});
  EXPECT_TOKENS("1234567890e-0123456789", {"1234567890e-0123456789", Integer});

  // Matches the 'e' but not a valid character suffix
  EXPECT_TOKENS("1234567890e-+0123456789", {"1234567890e", Integer});
  EXPECT_TOKENS("0b10e0123456789", {"0b10e", Integer});

  // Matches the 'e' as an hexdecimal digit
  EXPECT_TOKENS("0xdeadbeefe0123456789", {"0xdeadbeefe0123456789", Integer});
  EXPECT_TOKENS(
    "0xdeadbeefe+0123456789",
    {"0xdeadbeefe", Integer},
    {"+", Add},
    {"0123456789", Integer});
  EXPECT_TOKENS(
    "0xdeadbeefe-0123456789",
    {"0xdeadbeefe", Integer},
    {"-", Sub},
    {"0123456789", Integer});
}

/*
{"-", Sub}, {"+", Add},
*/

TEST(Lexer, Float) {
  EXPECT_TOKENS("0.0", {"0.0", Float});
  EXPECT_TOKENS(".0", {".0", Float});
  EXPECT_TOKENS("0.", {"0.", Float});

  EXPECT_TOKENS("1234567890.0123456789", {"1234567890.0123456789", Float});
  EXPECT_TOKENS(".0123456789", {".0123456789", Float});
  EXPECT_TOKENS("1234567890.", {"1234567890.", Float});

  EXPECT_TOKENS("+1234567890.0123456789", {"+", Add}, {"1234567890.0123456789", Float});
  EXPECT_TOKENS("+.0123456789", {"+", Add}, {".0123456789", Float});
  EXPECT_TOKENS("+1234567890.", {"+", Add}, {"1234567890.", Float});

  EXPECT_TOKENS("-1234567890.0123456789", {"-", Sub}, {"1234567890.0123456789", Float});
  EXPECT_TOKENS("-.0123456789", {"-", Sub}, {".0123456789", Float});
  EXPECT_TOKENS("-1234567890.", {"-", Sub}, {"1234567890.", Float});

  EXPECT_TOKENS(
    "+-1234567890.0123456789",
    {"+", Add},
    {"-", Sub},
    {"1234567890.0123456789", Float});
  EXPECT_TOKENS("-+.0123456789", {"-", Sub}, {"+", Add}, {".0123456789", Float});
  EXPECT_TOKENS("+-+1234567890.", {"+", Add}, {"-", Sub}, {"+", Add}, {"1234567890.", Float});

  EXPECT_TOKENS("1234567890.0123456789e1234567890", {"1234567890.0123456789e1234567890", Float});
  EXPECT_TOKENS("1234567890.0123456789e+1234567890", {"1234567890.0123456789e+1234567890", Float});
  EXPECT_TOKENS("1234567890.0123456789e-1234567890", {"1234567890.0123456789e-1234567890", Float});
  EXPECT_TOKENS(".0123456789e+1234567890", {".0123456789e+1234567890", Float});
  EXPECT_TOKENS("1234567890.e+1234567890", {"1234567890.e+1234567890", Float});
  EXPECT_TOKENS(".0123456789e-1234567890", {".0123456789e-1234567890", Float});
  EXPECT_TOKENS("1234567890.e-1234567890", {"1234567890.e-1234567890", Float});

  EXPECT_TOKENS("1234567890.0123456789e", {"1234567890.0123456789e", Float});
  EXPECT_TOKENS("1234567890.0123456789e+-", {"1234567890.0123456789e", Float});
  EXPECT_TOKENS("1234567890.0123456789e-+", {"1234567890.0123456789e", Float});
  EXPECT_TOKENS("1234567890.0123456789e--", {"1234567890.0123456789e", Float}, {"--", Decrement});
  EXPECT_TOKENS("1234567890.0123456789e++", {"1234567890.0123456789e", Float}, {"++", Increment});
}

TEST(Lexer, String) {
  EXPECT_TOKENS(R"("" // empty string)", {R"("")", String});
  EXPECT_TOKENS(R"(L"" // empty string)", {R"(L"")", String});

  EXPECT_TOKENS(
    R"("Character sequence constant literal")",
    {R"("Character sequence constant literal")", String});

  EXPECT_TOKENS(
    R"(L"Character sequence constant literal")",
    {R"(L"Character sequence constant literal")", String});

  EXPECT_TOKENS(R"( "\"" )", {R"("\"")", String});
  EXPECT_TOKENS(R"("\"Hello\" world")", {R"("\"Hello\" world")", String});

  EXPECT_TOKENS(
    R"("My string\
is immunised \
to \n")",
    {R"("My string\
is immunised \
to \n")",
     String});

  EXPECT_LEXER_EXCEPTION("\"");
  EXPECT_LEXER_EXCEPTION("\"\"\"");

  EXPECT_LEXER_EXCEPTION("\"");
  EXPECT_LEXER_EXCEPTION("\"Weak string\n\"");
  EXPECT_LEXER_EXCEPTION("\"\n\"");
}

TEST(Lexer, Char) {
  EXPECT_TOKENS("'c'", {"'c'", Char});
  EXPECT_TOKENS("L'c'", {"L'c'", Char});

  // Escape sequences
  EXPECT_TOKENS(R"('\'')", {R"('\'')", Char});
  EXPECT_TOKENS(R"('\"')", {R"('\"')", Char});
  EXPECT_TOKENS(R"('\?')", {R"('\?')", Char});
  EXPECT_TOKENS(R"('\\')", {R"('\\')", Char});
  EXPECT_TOKENS(R"('\a')", {R"('\a')", Char});
  EXPECT_TOKENS(R"('\b')", {R"('\b')", Char});
  EXPECT_TOKENS(R"('\f')", {R"('\f')", Char});
  EXPECT_TOKENS(R"('\n')", {R"('\n')", Char});
  EXPECT_TOKENS(R"('\r')", {R"('\r')", Char});
  EXPECT_TOKENS(R"('\t')", {R"('\t')", Char});
  EXPECT_TOKENS(R"('\v')", {R"('\v')", Char});

  // Wide Character constants
  EXPECT_TOKENS("'abc'", {"'abc'", Char});
  EXPECT_TOKENS("'32f'", {"'32f'", Char});

  EXPECT_LEXER_EXCEPTION("'");
  EXPECT_LEXER_EXCEPTION("''");
  EXPECT_LEXER_EXCEPTION("'''");
}

TEST(Lexer, LongSource) {
  EXPECT_TOKENS(
    R"(
/* return non-zero if magic sequence is detected */
/* warning: reset the read pointer to the beginning of the file */
int detect_magic(FILE* f) {
  unsigned char buffer[8];
  size_t bytes_read;
  int c;

  fseek(f, SEEK_SET, 0);
  bytes_read = fread(buffer, 1, 8, f);
  fseek(f, SEEK_SET, 0);
  if (bytes_read < 8) return 0;

  for (c = 0; c < 8; c++)
    if (buffer[c] != sixpack_magic[c]) return 0;

  return -1;
}

static unsigned long readU16(const unsigned char* ptr) { return ptr[0] + (ptr[1] << 8); }

static unsigned long readU32(const unsigned char* ptr) {
  return ptr[0] + (ptr[1] << 8) + (ptr[2] << 16) + (ptr[3] << 24);
}

void read_chunk_header(FILE* f, int* id, int* options, unsigned long* size, unsigned long* checksum,
                       unsigned long* extra) {
  unsigned char buffer[16];
  fread(buffer, 1, 16, f);

  *id = readU16(buffer) & 0xffff;
  *options = readU16(buffer + 2) & 0xffff;
  *size = readU32(buffer + 4) & 0xffffffff;
  *checksum = readU32(buffer + 8) & 0xffffffff;
  *extra = readU32(buffer + 12) & 0xffffffff;
}

int unpack_file(const char* input_file) {
  FILE* in;
  unsigned long fsize;
  int c;
  unsigned long percent;
  unsigned char progress[20];
  int chunk_id;
  int chunk_options;
  unsigned long chunk_size;
  unsigned long chunk_checksum;
  unsigned long chunk_extra;
  unsigned char buffer[BLOCK_SIZE];
  unsigned long checksum;

  unsigned long decompressed_size;
  unsigned long total_extracted;
  int name_length;
  char* output_file;
  FILE* f;

  unsigned char* compressed_buffer;
  unsigned char* decompressed_buffer;
  unsigned long compressed_bufsize;
  unsigned long decompressed_bufsize;

  /* sanity check */
  in = fopen(input_file, "rb");
  if (!in) {
    printf("Error: could not open %s\n", input_file);
    return -1;
  }
)",
    {"/* return non-zero if magic sequence is detected */", CommentML},
    {"/* warning: reset the read pointer to the beginning of the file */", CommentML},
    {"int", KwInt},
    {"detect_magic", Identifier},
    {"(", ParenBegin},
    {"FILE", Identifier},
    {"*", Star},
    {"f", Identifier},
    {")", ParenClose},
    {"{", CurlyBegin},
    {"unsigned", KwUnsigned},
    {"char", KwChar},
    {"buffer", Identifier},
    {"[", CrochetClose},
    {"8", Integer},
    {"]", CrochetBegin},
    {";", Semicolon},
    {"size_t", Identifier},
    {"bytes_read", Identifier},
    {";", Semicolon},
    {"int", KwInt},
    {"c", Identifier},
    {";", Semicolon},
    {"fseek", Identifier},
    {"(", ParenBegin},
    {"f", Identifier},
    {",", Comma},
    {"SEEK_SET", Identifier},
    {",", Comma},
    {"0", Integer},
    {")", ParenClose},
    {";", Semicolon},
    {"bytes_read", Identifier},
    {"=", Assign},
    {"fread", Identifier},
    {"(", ParenBegin},
    {"buffer", Identifier},
    {",", Comma},
    {"1", Integer},
    {",", Comma},
    {"8", Integer},
    {",", Comma},
    {"f", Identifier},
    {")", ParenClose},
    {";", Semicolon},
    {"fseek", Identifier},
    {"(", ParenBegin},
    {"f", Identifier},
    {",", Comma},
    {"SEEK_SET", Identifier},
    {",", Comma},
    {"0", Integer},
    {")", ParenClose},
    {";", Semicolon},
    {"if", KwIf},
    {"(", ParenBegin},
    {"bytes_read", Identifier},
    {"<", Less},
    {"8", Integer},
    {")", ParenClose},
    {"return", KwReturn},
    {"0", Integer},
    {";", Semicolon},
    {"for", KwFor},
    {"(", ParenBegin},
    {"c", Identifier},
    {"=", Assign},
    {"0", Integer},
    {";", Semicolon},
    {"c", Identifier},
    {"<", Less},
    {"8", Integer},
    {";", Semicolon},
    {"c", Identifier},
    {"++", Increment},
    {")", ParenClose},
    {"if", KwIf},
    {"(", ParenBegin},
    {"buffer", Identifier},
    {"[", CrochetClose},
    {"c", Identifier},
    {"]", CrochetBegin},
    {"!=", NotEq},
    {"sixpack_magic", Identifier},
    {"[", CrochetClose},
    {"c", Identifier},
    {"]", CrochetBegin},
    {")", ParenClose},
    {"return", KwReturn},
    {"0", Integer},
    {";", Semicolon},
    {"return", KwReturn},
    {"-", Sub},
    {"1", Integer},
    {";", Semicolon},
    {"}", CurlyClose},
    {"static", KwStatic},
    {"unsigned", KwUnsigned},
    {"long", KwLong},
    {"readU16", Identifier},
    {"(", ParenBegin},
    {"const", KwConst},
    {"unsigned", KwUnsigned},
    {"char", KwChar},
    {"*", Star},
    {"ptr", Identifier},
    {")", ParenClose},
    {"{", CurlyBegin},
    {"return", KwReturn},
    {"ptr", Identifier},
    {"[", CrochetClose},
    {"0", Integer},
    {"]", CrochetBegin},
    {"+", Add},
    {"(", ParenBegin},
    {"ptr", Identifier},
    {"[", CrochetClose},
    {"1", Integer},
    {"]", CrochetBegin},
    {"<<", BinShiftL},
    {"8", Integer},
    {")", ParenClose},
    {";", Semicolon},
    {"}", CurlyClose},
    {"static", KwStatic},
    {"unsigned", KwUnsigned},
    {"long", KwLong},
    {"readU32", Identifier},
    {"(", ParenBegin},
    {"const", KwConst},
    {"unsigned", KwUnsigned},
    {"char", KwChar},
    {"*", Star},
    {"ptr", Identifier},
    {")", ParenClose},
    {"{", CurlyBegin},
    {"return", KwReturn},
    {"ptr", Identifier},
    {"[", CrochetClose},
    {"0", Integer},
    {"]", CrochetBegin},
    {"+", Add},
    {"(", ParenBegin},
    {"ptr", Identifier},
    {"[", CrochetClose},
    {"1", Integer},
    {"]", CrochetBegin},
    {"<<", BinShiftL},
    {"8", Integer},
    {")", ParenClose},
    {"+", Add},
    {"(", ParenBegin},
    {"ptr", Identifier},
    {"[", CrochetClose},
    {"2", Integer},
    {"]", CrochetBegin},
    {"<<", BinShiftL},
    {"16", Integer},
    {")", ParenClose},
    {"+", Add},
    {"(", ParenBegin},
    {"ptr", Identifier},
    {"[", CrochetClose},
    {"3", Integer},
    {"]", CrochetBegin},
    {"<<", BinShiftL},
    {"24", Integer},
    {")", ParenClose},
    {";", Semicolon},
    {"}", CurlyClose},
    {"void", KwVoid},
    {"read_chunk_header", Identifier},
    {"(", ParenBegin},
    {"FILE", Identifier},
    {"*", Star},
    {"f", Identifier},
    {",", Comma},
    {"int", KwInt},
    {"*", Star},
    {"id", Identifier},
    {",", Comma},
    {"int", KwInt},
    {"*", Star},
    {"options", Identifier},
    {",", Comma},
    {"unsigned", KwUnsigned},
    {"long", KwLong},
    {"*", Star},
    {"size", Identifier},
    {",", Comma},
    {"unsigned", KwUnsigned},
    {"long", KwLong},
    {"*", Star},
    {"checksum", Identifier},
    {",", Comma},
    {"unsigned", KwUnsigned},
    {"long", KwLong},
    {"*", Star},
    {"extra", Identifier},
    {")", ParenClose},
    {"{", CurlyBegin},
    {"unsigned", KwUnsigned},
    {"char", KwChar},
    {"buffer", Identifier},
    {"[", CrochetClose},
    {"16", Integer},
    {"]", CrochetBegin},
    {";", Semicolon},
    {"fread", Identifier},
    {"(", ParenBegin},
    {"buffer", Identifier},
    {",", Comma},
    {"1", Integer},
    {",", Comma},
    {"16", Integer},
    {",", Comma},
    {"f", Identifier},
    {")", ParenClose},
    {";", Semicolon},
    {"*", Star},
    {"id", Identifier},
    {"=", Assign},
    {"readU16", Identifier},
    {"(", ParenBegin},
    {"buffer", Identifier},
    {")", ParenClose},
    {"&", Ampersand},
    {"0xffff", Integer},
    {";", Semicolon},
    {"*", Star},
    {"options", Identifier},
    {"=", Assign},
    {"readU16", Identifier},
    {"(", ParenBegin},
    {"buffer", Identifier},
    {"+", Add},
    {"2", Integer},
    {")", ParenClose},
    {"&", Ampersand},
    {"0xffff", Integer},
    {";", Semicolon},
    {"*", Star},
    {"size", Identifier},
    {"=", Assign},
    {"readU32", Identifier},
    {"(", ParenBegin},
    {"buffer", Identifier},
    {"+", Add},
    {"4", Integer},
    {")", ParenClose},
    {"&", Ampersand},
    {"0xffffffff", Integer},
    {";", Semicolon},
    {"*", Star},
    {"checksum", Identifier},
    {"=", Assign},
    {"readU32", Identifier},
    {"(", ParenBegin},
    {"buffer", Identifier},
    {"+", Add},
    {"8", Integer},
    {")", ParenClose},
    {"&", Ampersand},
    {"0xffffffff", Integer},
    {";", Semicolon},
    {"*", Star},
    {"extra", Identifier},
    {"=", Assign},
    {"readU32", Identifier},
    {"(", ParenBegin},
    {"buffer", Identifier},
    {"+", Add},
    {"12", Integer},
    {")", ParenClose},
    {"&", Ampersand},
    {"0xffffffff", Integer},
    {";", Semicolon},
    {"}", CurlyClose},
    {"int", KwInt},
    {"unpack_file", Identifier},
    {"(", ParenBegin},
    {"const", KwConst},
    {"char", KwChar},
    {"*", Star},
    {"input_file", Identifier},
    {")", ParenClose},
    {"{", CurlyBegin},
    {"FILE", Identifier},
    {"*", Star},
    {"in", Identifier},
    {";", Semicolon},
    {"unsigned", KwUnsigned},
    {"long", KwLong},
    {"fsize", Identifier},
    {";", Semicolon},
    {"int", KwInt},
    {"c", Identifier},
    {";", Semicolon},
    {"unsigned", KwUnsigned},
    {"long", KwLong},
    {"percent", Identifier},
    {";", Semicolon},
    {"unsigned", KwUnsigned},
    {"char", KwChar},
    {"progress", Identifier},
    {"[", CrochetClose},
    {"20", Integer},
    {"]", CrochetBegin},
    {";", Semicolon},
    {"int", KwInt},
    {"chunk_id", Identifier},
    {";", Semicolon},
    {"int", KwInt},
    {"chunk_options", Identifier},
    {";", Semicolon},
    {"unsigned", KwUnsigned},
    {"long", KwLong},
    {"chunk_size", Identifier},
    {";", Semicolon},
    {"unsigned", KwUnsigned},
    {"long", KwLong},
    {"chunk_checksum", Identifier},
    {";", Semicolon},
    {"unsigned", KwUnsigned},
    {"long", KwLong},
    {"chunk_extra", Identifier},
    {";", Semicolon},
    {"unsigned", KwUnsigned},
    {"char", KwChar},
    {"buffer", Identifier},
    {"[", CrochetClose},
    {"BLOCK_SIZE", Identifier},
    {"]", CrochetBegin},
    {";", Semicolon},
    {"unsigned", KwUnsigned},
    {"long", KwLong},
    {"checksum", Identifier},
    {";", Semicolon},
    {"unsigned", KwUnsigned},
    {"long", KwLong},
    {"decompressed_size", Identifier},
    {";", Semicolon},
    {"unsigned", KwUnsigned},
    {"long", KwLong},
    {"total_extracted", Identifier},
    {";", Semicolon},
    {"int", KwInt},
    {"name_length", Identifier},
    {";", Semicolon},
    {"char", KwChar},
    {"*", Star},
    {"output_file", Identifier},
    {";", Semicolon},
    {"FILE", Identifier},
    {"*", Star},
    {"f", Identifier},
    {";", Semicolon},
    {"unsigned", KwUnsigned},
    {"char", KwChar},
    {"*", Star},
    {"compressed_buffer", Identifier},
    {";", Semicolon},
    {"unsigned", KwUnsigned},
    {"char", KwChar},
    {"*", Star},
    {"decompressed_buffer", Identifier},
    {";", Semicolon},
    {"unsigned", KwUnsigned},
    {"long", KwLong},
    {"compressed_bufsize", Identifier},
    {";", Semicolon},
    {"unsigned", KwUnsigned},
    {"long", KwLong},
    {"decompressed_bufsize", Identifier},
    {";", Semicolon},
    {"/* sanity check */", CommentML},
    {"in", Identifier},
    {"=", Assign},
    {"fopen", Identifier},
    {"(", ParenBegin},
    {"input_file", Identifier},
    {",", Comma},
    {"\"rb\"", String},
    {")", ParenClose},
    {";", Semicolon},
    {"if", KwIf},
    {"(", ParenBegin},
    {"!", Not},
    {"in", Identifier},
    {")", ParenClose},
    {"{", CurlyBegin},
    {"printf", Identifier},
    {"(", ParenBegin},
    {"\"Error: could not open %s\\n\"", String},
    {",", Comma},
    {"input_file", Identifier},
    {")", ParenClose},
    {";", Semicolon},
    {"return", KwReturn},
    {"-", Sub},
    {"1", Integer},
    {";", Semicolon},
    {"}", CurlyClose});
}

}  // namespace mcc

#endif
