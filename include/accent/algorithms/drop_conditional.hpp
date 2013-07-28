#ifndef LIBACCENT_ALGORITHMS_DROP_CONDITIONAL_HPP
#define LIBACCENT_ALGORITHMS_DROP_CONDITIONAL_HPP

#include "accent/functional/not.hpp"
#include "accent/support/algorithm.hpp"

namespace accent { namespace algorithms {

  namespace detail {

    template <typename ReadableSinglePassRange, typename UnaryPredicate>
    ReadableSinglePassRange drop_while_r(ReadableSinglePassRange,
                                         UnaryPredicate,
                                         std::false_type);

    template <typename ReadableSinglePassRange, typename UnaryPredicate>
    ReadableSinglePassRange drop_while_r(ReadableSinglePassRange r,
                                         UnaryPredicate p,
                                         std::true_type) {
      while (r && p(const_(r))) {
        r.drop_front();
      }
      return r;
    }

  }

  template <typename ReadableSinglePassRange, typename UnaryPredicate>
  ReadableSinglePassRange drop_while_r(ReadableSinglePassRange r,
                                       UnaryPredicate p) {
    static_assert(support::SinglePassRange<ReadableSinglePassRange>(),
                  "drop_while requires a SinglePassRange");
    static_assert(support::ReadableRange<ReadableSinglePassRange>(),
                  "drop_while requires a ReadableRange");
    return detail::drop_while_r(r, p,
        support::bool_<support::SinglePassRange<ReadableSinglePassRange>() &&
                       support::ReadableRange<ReadableSinglePassRange>()>());
  }

  template <typename ReadableSinglePassRange, typename UnaryPredicate>
  ReadableSinglePassRange drop_while(ReadableSinglePassRange r,
                                     UnaryPredicate p) {
    return drop_while_r(r, functional::fronts(p));
  }

  template <typename ReadableSinglePassRange, typename UnaryPredicate>
  ReadableSinglePassRange drop_until_r(ReadableSinglePassRange r,
                                       UnaryPredicate p) {
    return drop_while_r(r, functional::not_(p));
  }

  template <typename ReadableSinglePassRange, typename UnaryPredicate>
  ReadableSinglePassRange drop_until(ReadableSinglePassRange r,
                                     UnaryPredicate p) {
    return drop_until_r(r, functional::fronts(p));
  }

}}

#endif
