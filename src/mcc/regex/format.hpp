#ifndef MCC_REGEX_FORMAT_HPP
#define MCC_REGEX_FORMAT_HPP

#include "raw_string.hpp"
#include "regex.hpp"
#include "state.hpp"
#include <algorithm>
#include <fmt/format.h>

namespace fmt {
using namespace mcc::regex;

template<>
struct formatter<Regex> {
  constexpr auto parse(format_parse_context &ctx) {
    auto end = std::find(ctx.begin(), ctx.end(), '}');

    // auto name_begin = std::find(ctx.begin(), end, '\'');
    // auto name_end = std::find(name_begin, end, '\'');

    // if (name_begin != end and name_end != end) {
    //   name = std::string_view{name_begin + 1, name_end};
    // }

    return end;
  }

  constexpr auto format(const Regex &regex, auto &ctx) {
    format_to(ctx.out(), "digraph {} {{\n", "regex");

    if (regex.head()) {
      auto header = R"(rankdir=LR;bgcolor="#F9F9F9";compound=true)";
      auto src = Raw{regex.src(), 2};
      auto head_index = regex.head()->index();
      auto head_state = regex.head()->state();

      format_to(ctx.out(), "{}\n", header);
      format_to(ctx.out(), R"("{}" [shape="none"]{})", src, '\n');
      format_to(ctx.out(), R"("{}" -> {} [label="{}"]{})", src, head_index, head_state, '\n');

      auto stack = regex.stack();

      for (const Node &node : regex.stack()) {
        format_to(ctx.out(), "{}", node);
      }
    }

    return format_to(ctx.out(), "}}");
  }

  std::string_view name;
};

template<>
struct formatter<Node> {
  constexpr auto parse(format_parse_context &ctx) {
    return ctx.begin();
  }

  auto shape(const Node &node) const -> std::string_view {
    return node.ok() ? "circle" : "square";
  }

  constexpr auto embed(const Node &node, const Node *sequence, std::string_view header, auto &ctx) {
    format_to(
      ctx.out(),
      R"(subgraph cluster_{} {{
{}
{}
}}
)",
      sequence->index(),
      header,
      node);

    return format_to(ctx.out(), "{} -> {}\n", node.index(), sequence->index());
  }

  constexpr auto format(const Node &node, auto &ctx) {
    format_to(ctx.out(), R"("{}" [shape="{}"]{})", node.index(), shape(node), '\n');

    switch (node.state().option()) {
    case Option::Not: {
      auto [sequence] = std::get<Not>(node.state().variant());
      embed(node, sequence, R"(style=filled;bgcolor="#FBF3F3")", ctx);
    }
    case Option::Dash: {
      auto [sequence] = std::get<Dash>(node.state().variant());
      embed(node, sequence, R"(style=filled;bgcolor="#F4FDFF")", ctx);
    }

    default: break;
    }

    for (const Node *edge : node.edges()) {
      constexpr std::string_view fmt = R"({} -> {} [label="{}"]{})";
      format_to(ctx.out(), fmt, node.index(), edge->index(), edge->state(), '\n');
    }

    return ctx.out();
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
