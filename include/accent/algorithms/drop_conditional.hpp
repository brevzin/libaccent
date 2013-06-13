#ifndef LIBACCENT_ALGORITHMS_DROP_CONDITIONAL_HPP
#define LIBACCENT_ALGORITHMS_DROP_CONDITIONAL_HPP

#include "accent/support/concepts.hpp"
#include "accent/support/utility.hpp"

namespace accent { namespace algorithms {

  namespace detail {

    template <typename ReadableSinglePassRange, typename UnaryPredicate>
    ReadableSinglePassRange drop_while(ReadableSinglePassRange, UnaryPredicate,
                                       std::false_type);

    template <typename ReadableSinglePassRange, typename UnaryPredicate>
    ReadableSinglePassRange drop_while(ReadableSinglePassRange r,
                                       UnaryPredicate p,
                                       std::true_type) {
      while (r && p(r.front())) {
        r.drop_front();
      }
      return r;
    }

  }

  template <typename ReadableSinglePassRange, typename UnaryPredicate>
  ReadableSinglePassRange drop_while(ReadableSinglePassRange r,
                                     UnaryPredicate p) {
    static_assert(support::SinglePassRange<ReadableSinglePassRange>(),
                  "drop_while requires a SinglePassRange");
    static_assert(support::ReadableRange<ReadableSinglePassRange>(),
                  "drop_while requires a ReadableRange");
    return detail::drop_while(r, p,
        support::bool_<support::SinglePassRange<ReadableSinglePassRange>() &&
                       support::ReadableRange<ReadableSinglePassRange>()>());
  }

}}

#endif
