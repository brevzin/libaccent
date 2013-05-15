#ifndef LIBACCENT_SUPPORT_CONCEPTS_HPP
#define LIBACCENT_SUPPORT_CONCEPTS_HPP

#include "accent/tags.hpp"
#include <type_traits>

namespace accent { namespace support {

  namespace detail {

    // Test for conversion to bool and operator !.
    template <typename T>
    std::true_type has_proper_boolean_conversion_helper(const T* t,
        decltype(*t ? true : true) = true);
    std::false_type has_proper_boolean_conversion_helper(...);
    template <typename T>
    struct has_proper_boolean_conversion
      : decltype(has_proper_boolean_conversion_helper((T*)0))
    {};

    // Test for nested type traversal and its convertibility to T.
    template <typename T, typename R>
    typename std::is_base_of<T, typename R::traversal>::type
        traversal_supports_helper(R*);
    template <typename T>
    std::false_type traversal_supports_helper(...);
    template <typename T, typename R>
    struct traversal_supports
      : decltype(traversal_supports_helper<T>((R*)0))
    {};

    // Test for empty() that returns convertible to bool.
    template <typename R>
    auto has_empty_helper(const R* r)
        -> typename std::is_convertible<decltype(r->empty()), bool>::type;
    std::false_type has_empty_helper(...);
    template <typename R>
    struct has_empty
      : decltype(has_empty_helper((R*)0))
    {};

    // Test for nested type value_type.
    template <typename R>
    std::true_type has_value_type_helper(const R* r,
                                         const typename R::value_type* = 0);
    std::false_type has_value_type_helper(...);
    template <typename R>
    struct has_value_type
      : decltype(has_value_type_helper((R*)0))
    {};

    // Test for front() that returns non-void.
    template <typename R>
    auto has_front_helper(const R* r)
        -> typename std::conditional<
            std::is_same<decltype(r->front()), void>::value,
            std::false_type, std::true_type>::type;
    std::false_type has_front_helper(...);
    template <typename R>
    struct has_front
      : decltype(has_front_helper((R*)0))
    {};

    // Test for drop_front().
    template <typename R>
    std::true_type has_drop_front_helper(R* r,
        decltype(r->drop_front())* = 0);
    std::false_type has_drop_front_helper(...);
    template <typename R>
    struct has_drop_front
      : decltype(has_drop_front_helper((R*)0))
    {};

  }

  template <typename R>
  constexpr bool SinglePassRange() {
    return std::is_copy_constructible<R>::value &&
           std::is_copy_assignable<R>::value &&
           detail::has_proper_boolean_conversion<R>::value &&
           detail::traversal_supports<single_pass_traversal_tag, R>::value &&
           detail::has_empty<R>::value &&
           detail::has_value_type<R>::value &&
           detail::has_front<R>::value &&
           detail::has_drop_front<R>::value;
  }

}}

#endif
