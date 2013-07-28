#ifndef LIBACCENT_ALGORITHMS_FOR_EACH_HPP
#define LIBACCENT_ALGORITHMS_FOR_EACH_HPP

#include "accent/support/algorithm.hpp"

namespace accent { namespace algorithms {

  namespace detail {

    template <typename ReadableSinglePassRange, typename UnaryFunctor>
    UnaryFunctor for_each_r(ReadableSinglePassRange, UnaryFunctor,
                          std::false_type);

    template <typename ReadableSinglePassRange, typename UnaryFunctor>
    UnaryFunctor for_each_r(ReadableSinglePassRange r, UnaryFunctor f,
                          std::true_type) {
      for (; r; r.drop_front()) {
        f(const_(r));
      }
      return f;
    }

  }

  template <typename ReadableSinglePassRange, typename UnaryFunctor>
  UnaryFunctor for_each_r(ReadableSinglePassRange r, UnaryFunctor f) {
    static_assert(support::SinglePassRange<ReadableSinglePassRange>(),
                  "for_each requires a SinglePassRange");
    static_assert(support::ReadableRange<ReadableSinglePassRange>(),
                  "for_each requires a ReadableRange");
    return detail::for_each_r(r, f,
        support::bool_<support::SinglePassRange<ReadableSinglePassRange>() &&
                       support::ReadableRange<ReadableSinglePassRange>()>());
  }

  template <typename ReadableSinglePassRange, typename UnaryFunctor>
  UnaryFunctor for_each(ReadableSinglePassRange r, UnaryFunctor f) {
    return algorithms::for_each_r(r, functional::fronts(f)).base();
  }

}}

#endif
