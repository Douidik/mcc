#ifndef MCC_VARIANT_HPP
#define MCC_VARIANT_HPP

#include "mcc.hpp"
#include <concepts>

namespace mcc {

namespace variant {
  struct None {};

  template<size_t S, typename H, typename... Ts>
  struct Option {
    H data;
    None next;
  };

  template<size_t S, typename H, typename N, typename... Ts>
  struct Option<S, H, N, Ts...> {
    H data;
    Option<S + 1, N, Ts...> next;
  };

  // TODO: Template option constness
  template<typename M, size_t S, typename H, typename... Ts>
  constexpr auto option_by_type(const Option<S, H, Ts...> &option) -> const M & {
    if constexpr (std::same_as<M, H>) {
      return option.data;
    }
    static_assert(!std::same_as<decltype(option.next), None>, "Variant option does not exists");
    return option_by_type<M, S + 1, Ts...>(option.next);
  }

  template<typename M, size_t S, typename H, typename... Ts>
  constexpr auto option_by_type(Option<S, H, Ts...> &option) -> M & {
    if constexpr (std::same_as<M, H>) {
      return option.data;
    }
    static_assert(!std::same_as<decltype(option.next), None>, "Variant option does not exists");
    return option_by_type<M, S + 1, Ts...>(option.next);
  }

  template<size_t I, size_t S, typename H, typename... Ts>
  constexpr auto option_by_index(const Option<S, H, Ts...> &option) -> decltype(auto) {
    if constexpr (I == S) {
      return option;
    }

    static_assert(!std::same_as<decltype(option.next), None>, "Variant option does not exists");
    return option_by_index<I, S + 1, Ts...>(option.next);
  }

  template<size_t I, size_t S, typename H, typename... Ts>
  constexpr auto option_by_index(Option<S, H, Ts...> &option) -> decltype(auto) {
    if constexpr (I == S) {
      return option;
    }

    static_assert(!std::same_as<decltype(option.next), None>, "Variant option does not exists");
    return option_by_index<I, S + 1, Ts...>(option.next);
  }

}  // namespace variant

template<typename... Ts>
class Variant {
  static_assert(sizeof...(Ts) > 0, "Variant cannot be empty");

public:
  template<typename T>
  constexpr auto has() const -> bool {
    return true;
  }

  template<typename T>
  constexpr auto as() const -> const T & {
    return variant::option_by_type<T>(m_head);
  }

  template<typename T>
  constexpr auto as() -> T & {
    return variant::option_by_type<T>(m_head);
  }

  constexpr auto index() const -> size_t {
    return m_index;
  }

private:
  size_t m_index;
  variant::Option<0, Ts...> m_head;
};

};  // namespace mcc

#endif
