#ifndef LIBACCENT_SUPPORT_CONCEPTS_HPP
#define LIBACCENT_SUPPORT_CONCEPTS_HPP

#include "accent/tags.hpp"
#include "accent/support/tag_functions.hpp"
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

    // Test that front() is convertible to value_type
    template <typename R>
    auto front_result_helper(const R* r) -> decltype(r->front());
    struct front_result_dummy {};
    front_result_dummy front_result_helper(...);
    template <typename R>
    struct front_converts_to_value_type
      : std::is_convertible<decltype(front_result_helper((R*)0)),
                            value_type_of<R>>
    {};

    // Test that front() can be assigned to from value_type
    template <typename R>
    std::true_type front_assignable_helper(const R* r,
        typename std::decay<
            decltype(r->front() = std::declval<value_type_of<R>>())
        >::type* = 0);
    std::false_type front_assignable_helper(...);
    template <typename R>
    struct front_assignable_from_value_type
      : decltype(front_assignable_helper((R*)0))
    {};

    // Test for at_front() that returns position
    template <typename R>
    auto has_at_front_helper(const R* r)
        -> typename std::is_same<typename R::position,
                                 decltype(r->at_front())>::type;
    std::false_type has_at_front_helper(...);
    template <typename R>
    struct has_at_front
      : decltype(has_at_front_helper((R*)0))
    {};

    // Test for from(position) that returns R
    template <typename R>
    auto has_from_helper(const R* r)
        -> typename std::is_same<R, decltype(r->from(r->at_front()))>::type;
    std::false_type has_from_helper(...);
    template <typename R>
    struct has_from : decltype(has_from_helper((R*)0)) {};

    // Test for until(position). Can't test here that it's a forward range.
    template <typename R>
    std::true_type has_until_helper(const R* r,
                                    decltype(r->until(r->at_front()))* = 0);
    std::false_type has_until_helper(...);
    template <typename R>
    struct has_until : decltype(has_until_helper((R*)0)) {};

    // Get the return type of until(position), or else void.
    template <typename R>
    auto until_return_type(const R* r) -> decltype(r->until(r->at_front()));
    void until_return_type(...);

    // Check that a range's position type is the same as the until-range's.
    template <typename R>
    struct until_consistent_position
      : std::is_same<position_of<R>,
                     position_of<decltype(until_return_type((R*)0))>>
    {};

  }

  template <typename P>
  constexpr bool Position() {
    return !std::is_same<P, void>::value;
  };

  template <typename R>
  constexpr bool SinglePassRange() {
    return std::is_copy_constructible<R>::value &&
           std::is_copy_assignable<R>::value &&
           detail::has_proper_boolean_conversion<R>::value &&
           traversal_supports<single_pass_traversal_tag, R>::value &&
           detail::has_empty<R>::value &&
           detail::has_value_type<R>::value &&
           detail::has_front<R>::value &&
           detail::has_drop_front<R>::value;
  }

  namespace detail {

    template <typename R>
    constexpr bool ForwardRangeWithoutUntilTypeCheck() {
      return SinglePassRange<R>() &&
             traversal_supports<forward_traversal_tag, R>::value &&
             Position<position_of<R>>() &&
             has_at_front<R>::value &&
             has_from<R>::value &&
             has_until<R>::value;
    }

  }

  template <typename R>
  constexpr bool ForwardRange() {
    // We do only a partial check of the until return type, to avoid going
    // into endless recursion.
    return detail::ForwardRangeWithoutUntilTypeCheck<R>() &&
           detail::ForwardRangeWithoutUntilTypeCheck<
               decltype(detail::until_return_type((R*)0))>() &&
           detail::until_consistent_position<R>::value;
  }

  template <typename R>
  constexpr bool ReadableRange() {
    return detail::front_converts_to_value_type<R>::value;
  }

  template <typename R>
  constexpr bool WriteableRange() {
    return detail::front_assignable_from_value_type<R>::value;
  }

}}

#endif
