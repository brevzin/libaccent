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

  }

  template <typename R>
  constexpr bool SinglePassRange() {
    return std::is_copy_constructible<R>::value &&
           std::is_copy_assignable<R>::value &&
           detail::has_proper_boolean_conversion<R>::value &&
           detail::traversal_supports<single_pass_traversal_tag, R>::value;
  }

}}

#endif
