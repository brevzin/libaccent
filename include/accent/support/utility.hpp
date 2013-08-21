#ifndef LIBACCENT_SUPPORT_UTILITY_HPP
#define LIBACCENT_SUPPORT_UTILITY_HPP

#include <type_traits>

namespace accent { namespace support {

  template <typename T, typename... Ts>
  using head = T;

  template <bool V>
  using bool_ = std::integral_constant<bool, V>;

  template <bool... Bs> struct all;
  template <> struct all<> : support::bool_<true> {};
  template <bool B, bool... Bs> struct all<B, Bs...>
      : support::bool_<B && all<Bs...>::value> {};

  template <typename T>
  const T& const_(const T& t) {
    return t;
  }

}}

#endif
