#ifndef LIBACCENT_ALGORITHMS_FOR_EACH_HPP
#define LIBACCENT_ALGORITHMS_FOR_EACH_HPP

#include "accent/support/algorithm.hpp"

namespace accent { namespace algorithms {

  namespace detail {

    template <typename ReadableSinglePassRange, typename UnaryFunctor>
    UnaryFunctor for_each(ReadableSinglePassRange, UnaryFunctor,
                          std::false_type);

    template <typename ReadableSinglePassRange, typename UnaryFunctor>
    UnaryFunctor for_each(ReadableSinglePassRange r, UnaryFunctor f,
                          std::true_type) {
      for (; r; r.drop_front()) {
        f(r.front());
      }
      return f;
    }

  }

  template <typename ReadableSinglePassRange, typename UnaryFunctor>
  UnaryFunctor for_each(ReadableSinglePassRange r, UnaryFunctor f) {
    static_assert(support::SinglePassRange<ReadableSinglePassRange>(),
                  "for_each requires a SinglePassRange");
    static_assert(support::ReadableRange<ReadableSinglePassRange>(),
                  "for_each requires a ReadableRange");
    return detail::for_each(r, f,
        support::bool_<support::SinglePassRange<ReadableSinglePassRange>() &&
                       support::ReadableRange<ReadableSinglePassRange>()>());
  }

}}

#endif
