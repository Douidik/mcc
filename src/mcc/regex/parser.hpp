#ifndef MCC_REGEX_PARSER_HPP
#define MCC_REGEX_PARSER_HPP

#include "mcc.hpp"
#include <vector>

namespace mcc::regex {
class Stack;
class Node;

class Parser {
public:
  Parser(std::string_view src, Stack &stack);
  auto parse() -> Node *;

private:
  auto parse_new_token() -> Node *;

  auto parse_sequence_src() -> std::string_view;
  auto parse_binary_op(char op) -> std::pair<Node *, Node *>;
  auto parse_pre_op(char op) -> Node *;
  auto parse_post_op(char op) -> Node *;
  
  auto parse_set(std::string_view set) -> Node *;
  auto parse_range() -> Node *;
  auto parse_any() -> Node *;
  auto parse_text(char q) -> Node *;
  auto parse_sequence() -> Node *;
  auto parse_dash() -> Node *;
  auto parse_not() -> Node *;
  auto parse_or() -> Node *;
  auto parse_quest() -> Node *;
  auto parse_star() -> Node *;
  auto parse_plus() -> Node *;
  auto parse_wave() -> Node *;

  auto exception(std::string_view desc) -> Exception;

  std::vector<Node *> m_sequences;
  Stack &m_stack;
  const char *m_token;
  std::string_view m_src;
};

}  // namespace mcc::regex

#endif
