#ifndef MCC_REGEX_FORMAT_HPP
#define MCC_REGEX_FORMAT_HPP

#include "raw_string.hpp"
#include "regex.hpp"
#include "state.hpp"
#include <algorithm>
#include <fmt/format.h>
#include <utility>

namespace fmt {
using namespace mcc::regex;

template<>
struct formatter<Regex> {
  std::string_view name = "";

  constexpr auto parse(format_parse_context &ctx) {
    auto end = std::find(ctx.begin(), ctx.end(), '}');
    name     = std::string_view{ctx.begin(), end};
    return end;
  }

  constexpr auto format(const Regex &regex, auto &ctx) {
    format_to(ctx.out(), "strict digraph {{\n");

    if (regex.head()) {
      auto header     = R"(rankdir=LR;bgcolor="#F9F9F9";compound=true)";
      auto head_ptr   = ptr(regex.head());
      auto head_state = regex.head()->state();
      auto start      = Raw{name != "" ? name : regex.src(), 2};

      format_to(ctx.out(), "{}\n", header);
      format_to(ctx.out(), R"("{}" [shape="none"]{})", start, '\n');
      format_to(ctx.out(), R"("{}" -> "{}" [label="{}"]{})", start, head_ptr, head_state, '\n');

      for (const Node *node : regex.head()->members()) {
        format_to(ctx.out(), "{}", *node);
      }
    }

    return format_to(ctx.out(), "}}");
  }
};

template<>
struct formatter<Node> {
  constexpr auto parse(format_parse_context &ctx) {
    return ctx.begin();
  }

  auto define(const Node &node, auto &ctx) {
    constexpr auto fmt = R"("{}" [shape="{}",label="{}"]{})";
    auto shape         = node.branch() ? "square" : "circle";
    return format_to(ctx.out(), fmt, ptr(&node), shape, node.index(), '\n');
  }

  auto connect(const Node &from, const Node &into, auto &ctx) {
    constexpr std::string_view fmt = R"("{}" -> "{}" [label="{}"]{})";
    format_to(ctx.out(), fmt, ptr(&from), ptr(&into), into.state(), '\n');
  }

  constexpr auto
  format_subgraph(const Node &node, Node *sub, std::string_view header, char op, auto &ctx) {
    format_to(ctx.out(), "subgraph cluster_{} {{\n", ptr(&node));
    format_to(ctx.out(), "{}\n", header);
    define(node, ctx);
    connect(node, *sub, ctx);
    for (const Node *subnode : sub->members()) {
      format_to(ctx.out(), "{}", *subnode);
    }
    format_to(ctx.out(), "{}", *sub);
    format_to(ctx.out(), "}}\n");

    const Node *end = sub->end();
    for (const Node *edge : node.edges()) {
      connect(*end, *edge, ctx);
    }

    return ctx.out();
  }

  constexpr auto format_node(const Node &node, auto &ctx) {
    for (const Node *edge : node.edges()) {
      connect(node, *edge, ctx);
    }
    return define(node, ctx);
  }

  constexpr auto format(const Node &node, auto &ctx) {
    switch (node.state().option()) {
    case Option::None: {
      return ctx.out();
    };

    case Option::Not: {
      auto [sequence] = std::get<regex::Not>(node.state().variant());
      auto header     = R"(style=filled;bgcolor="#FBF3F3")";
      return format_subgraph(node, sequence, header, '/', ctx);
    } break;

    case Option::Dash: {
      auto [sequence] = std::get<regex::Dash>(node.state().variant());
      auto header     = R"(style=filled;bgcolor="#F4FDFF")";
      return format_subgraph(node, sequence, header, '/', ctx);
    } break;

    default: {
      return format_node(node, ctx);
    }
    }
  }
};

template<>
struct formatter<State> {
  constexpr auto parse(format_parse_context &ctx) {
    return ctx.begin();
  }

  constexpr auto format(const State &state, auto &ctx) {
    switch (state.option()) {
    case Option::Epsilon: return format_to(ctx.out(), "&Sigma;");
    case Option::Any: return format_to(ctx.out(), "&alpha;");
    case Option::None: return format_to(ctx.out(), "&times;");
    case Option::Not: return format_to(ctx.out(), "!");
    case Option::Dash: return format_to(ctx.out(), "/");

    case Option::Text: {
      return format_to(ctx.out(), "'{}'", Raw{std::get<Text>(state.variant()).content, 2});
    }

    case Option::Set: {
      auto set = std::get<Set>(state.variant()).content;

      if (set.size() < 1) {
        return format_to(ctx.out(), "[]");
      }
      if (set.size() < 2) {
        return format_to(ctx.out(), "[{}]", Raw{set, 2});
      }

      Raw a{set.substr(0, 1), 2};
      Raw b{set.substr(set.size() - 1, 1), 2};
      return format_to(ctx.out(), "[{}..{}]", a, b);
    }

    case Option::Range: {
      auto [a, b] = std::get<Range>(state.variant());
      return format_to(ctx.out(), "[{}-{}]", Raw{{&a, 1}, 2}, Raw{{&b, 1}, 2});
    }
    }
    return ctx.out();
  }
};

}  // namespace fmt

#endif
