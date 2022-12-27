#include "parser.hpp"
#include "format.hpp"
#include "regex.hpp"
#include <algorithm>

namespace mcc::regex {

Parser::Parser(std::string_view src, Stack &stack) :
  m_stack(stack),
  m_sequences(),
  m_src(src),
  m_token(src.end()) {}

auto Parser::parse() -> Node * {
  for (m_token = m_src.begin(); m_token < m_src.end(); m_token++) {
    auto sequence = parse_new_token();
    if (sequence) m_sequences.push_back(sequence);
  }

  for (size_t i = 1; i < m_sequences.size(); i++) {
    m_sequences[0]->merge(m_sequences[i]);
  }

  if (!m_sequences.empty()) {
    return m_sequences[0];
  }
  return {};
}

auto Parser::parse_new_token() -> Node * {
  if (m_token > m_src.end() - 1) {
    return {};
  }

  switch (*m_token) {
  case ' ':
  case '\f':
  case '\n':
  case '\r':
  case '\t':
  case '\v': m_token++; return parse_new_token();

  case '_': return parse_set("\n \v\b\f\t");
  case 'a': return parse_set("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
  case 'o': return parse_set("!#$%&()*+,-./:;<=>?@[\\]^`{|}~");
  case 'n': return parse_set("0123456789");
  case 'Q': return parse_set("\"");
  case 'q': return parse_set("'");

  case '[': return parse_range();
  case '^': return parse_any();
  case '!': return parse_not();
  case '\'': return parse_text('\'');
  case '`': return parse_text('`');
  case '{': return parse_sequence();
  case '|': return parse_or();
  case '?': return parse_quest();
  case '*': return parse_star();
  case '+': return parse_plus();

  case '}': throw exception("unmatched sequence brace, missing <{> operator");
  case ']': throw exception("unmatched set brace, missing <[> operator");
  default: throw exception("unrecognized token in regex, none of [_aonQq^'{}!|?*+~]");
  }
}

auto Parser::parse_sequence_src() -> std::string_view {
  i32 depth = 0;

  const char *begin = m_token + 1;
  const char *end = std::find_if(m_token, m_src.end(), [&depth](char token) -> bool {
    switch (token) {
    case '{': depth++; break;
    case '}': depth--; break;
    }
    return depth < 1;
  });

  if (end == m_src.end()) {
    throw exception("unmatched sequence brace, missing <}> character");
  }

  return {begin, m_token = end};
}

auto Parser::parse_binary_op(char op) -> std::pair<Node *, Node *> {
  return {parse_pre_op(op), parse_post_op(op)};
}

auto Parser::parse_pre_op(char op) -> Node * {
  if (m_sequences.empty()) {
    throw exception(fmt::format("missing pre-operand for <{}> operator", op));
  }

  auto operand = m_sequences.back();
  m_sequences.pop_back();
  return operand;
}

auto Parser::parse_post_op(char op) -> Node * {
  m_token++;
  auto head = parse_new_token();

  if (!head) {
    throw exception(fmt::format("missing post-operand for <{}> operator", op));
  }

  return head;
}

auto Parser::parse_set(std::string_view set) -> Node * {
  return m_stack.push(Set{set}, 0);
}

auto Parser::parse_range() -> Node * {
  static const Regex range_format = "'[' ^ '-' ^ ']'";

  // Range format, with: ^ an ascii character, [ & ] range bounds
  if (!range_format.match(m_token, m_src.end())) {
    throw exception("range does not match the format: '[' ^ '-' ^ ']'");
  }

  char a = m_token[1];
  char b = m_token[3];
  m_token = &m_token[4];

  return m_stack.push(Range{a, b}, 0);
}

auto Parser::parse_any() -> Node * {
  return m_stack.push(Any{}, 0);
}

auto Parser::parse_text(char q) -> Node * {
  const char *begin = m_token + 1;
  const char *end = std::find(begin, m_src.end(), q);

  if (end == m_src.end()) {
    throw exception("unmatched string literal quote, missing ending <'> character");
  }

  m_token = end;
  std::string_view content(begin, end);
  return m_stack.push(Text{content}, 0);
}

auto Parser::parse_sequence() -> Node * {
  Parser parser{parse_sequence_src(), m_stack};
  return parser.parse();
}

auto Parser::parse_dash() -> Node * {
  return m_stack.push(Dash{parse_post_op('/')}, 0);
}

auto Parser::parse_not() -> Node * {
  return m_stack.push(Not{parse_post_op('!')}, 0);
}

/*
  Control flow structures:
  
  a: 1st binary operand
  b: 2nd binary operand
  o: unary operand
  $: epsilon
  ^: any
  >: edge
*/

auto Parser::parse_or() -> Node * {
  //   > a
  // $
  //   > b
  auto [a, b] = parse_binary_op('|');

  auto head = m_stack.push(Epsilon{}, 0);
  head->push(a);
  head->push(b);

  return head;
}

auto Parser::parse_quest() -> Node * {
  //   > o
  // $
  //   > $'
  auto head = m_stack.push(Epsilon{}, 0);

  head->merge(parse_pre_op('?'));
  head->push(m_stack.push(Epsilon{}));

  return head;
}

auto Parser::parse_star() -> Node * {
  //   > o > $
  // $
  //   > $'
  auto head = m_stack.push(Epsilon{}, 0);

  head->merge(parse_pre_op('*'))->concat(head);
  head->push(m_stack.push(Epsilon{}));

  return head;
}

auto Parser::parse_plus() -> Node * {
  // $ > e > $
  auto head = parse_pre_op('+');
  return head->concat(head);
}

auto Parser::exception(std::string_view desc) -> Exception {
  return Exception{
    "regex exception",
    R"(with {{
{}
{:>{}} {}
}})",
    m_src,
    '^',
    m_token - m_src.begin() + 1,
    desc,
  };
}

}  // namespace mcc::regex
