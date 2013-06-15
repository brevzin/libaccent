#ifndef LIBACCENT_ALGORITHMS_FIND_HPP
#define LIBACCENT_ALGORITHMS_FIND_HPP

#include "accent/algorithms/drop_conditional.hpp"
#include "accent/support/algorithm.hpp"

namespace accent { namespace algorithms {

  namespace detail {

    template <typename ReadableForwardRange, typename UnaryPredicate>
    void find(ReadableForwardRange, UnaryPredicate, std::false_type);

    template <typename ReadableForwardRange, typename UnaryPredicate>
    support::position_of<ReadableForwardRange>
    find(ReadableForwardRange r, UnaryPredicate p, std::true_type) {
      // Don't want to pick up the detail version here.
      r = algorithms::drop_until(r, p);
      return r.at_front();
    }

  }

  template <typename ReadableForwardRange, typename UnaryPredicate>
  support::position_of<ReadableForwardRange>
  find(ReadableForwardRange r, UnaryPredicate p) {
    static_assert(support::ForwardRange<ReadableForwardRange>(),
                  "find requires a ForwardRange");
    static_assert(support::ReadableRange<ReadableForwardRange>(),
                  "find requires a ReadableRange");
    return detail::find(r, p,
        support::bool_<support::ForwardRange<ReadableForwardRange>() &&
                       support::ReadableRange<ReadableForwardRange>()>());
  }

}}

#endif
