#ifndef LIBACCENT_SUPPORT_TAG_FUNCTIONS_HPP
#define LIBACCENT_SUPPORT_TAG_FUNCTIONS_HPP

#include "accent/tags.hpp"
#include <type_traits>

namespace accent { namespace support {
  namespace detail {

    struct dummy {};

    template <typename A, typename... Bs>
    struct minimum_tag_impl;

    template <typename A>
    struct minimum_tag_impl<A>
    {
      using type = A;
    };

    template <typename A, typename B>
    struct minimum_tag_impl<A, B>
    {
      using type = typename std::conditional<std::is_base_of<A, B>::value,
                                             A, B>::type;
    };

    template <typename A, typename B, typename... Bs>
    struct minimum_tag_impl<A, B, Bs...>
    {
      using type = typename minimum_tag_impl<
                              typename minimum_tag_impl<A, B>::type,
                              Bs...>::type;
    };

    // Test for nested type traversal and its convertibility to T.
    template <typename T, typename R>
    typename std::is_base_of<T, typename R::traversal>::type
        traversal_supports_helper(R*);
    template <typename T>
    std::false_type traversal_supports_helper(...);

    // Grab the nested type traversal, if any.
    template <typename R>
    typename R::traversal traversal_of_helper(R*);
    dummy traversal_of_helper(...);

    // Grab the nested type value_type, if any.
    template <typename R>
    typename R::value_type value_type_of_helper(R*);
    dummy value_type_of_helper(...);
  }

  template <typename T, typename R>
  struct traversal_supports
    : decltype(detail::traversal_supports_helper<T>((R*)0))
  {};

  template <typename A, typename... Bs>
  using minimum_tag = typename detail::minimum_tag_impl<A, Bs...>::type;

  template <typename R>
  using traversal_of = decltype(detail::traversal_of_helper((R*)0));

  template <typename R>
  using value_type_of = decltype(detail::value_type_of_helper((R*)0));

}}

#endif
