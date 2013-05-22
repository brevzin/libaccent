#ifndef LIBACCENT_ALGORITHMS_ANY_OF_HPP
#define LIBACCENT_ALGORITHMS_ANY_OF_HPP

#include "accent/support/concepts.hpp"
#include "accent/support/tag_functions.hpp"
#include "accent/support/utility.hpp"

namespace accent { namespace algorithms {

  namespace detail {

    template <typename ReadableSinglePassRange, typename Predicate>
    bool any_of(ReadableSinglePassRange, Predicate, std::false_type);

    template <typename ReadableSinglePassRange, typename Predicate>
    bool any_of(ReadableSinglePassRange r, Predicate p, std::true_type) {
      for (; r; r.drop_front()) {
        if (p(r.front()))
          return true;
      }
      return false;
    }

  }

  template <typename ReadableSinglePassRange, typename Predicate>
  bool any_of(ReadableSinglePassRange r, Predicate p) {
    static_assert(support::SinglePassRange<ReadableSinglePassRange>(),
                  "any_of requires a SinglePassRange");
    static_assert(support::ReadableRange<ReadableSinglePassRange>(),
                  "any_of requires a ReadableRange");
    return detail::any_of(r, p,
        support::bool_<support::SinglePassRange<ReadableSinglePassRange>() &&
                       support::ReadableRange<ReadableSinglePassRange>()>());
  }

}}

#endif
