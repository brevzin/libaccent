#ifndef LIBACCENT_ALGORITHMS_EQUAL_HPP
#define LIBACCENT_ALGORITHMS_EQUAL_HPP

#include "accent/support/algorithm.hpp"
#include <functional>

namespace accent { namespace algorithms {

  namespace detail {

    template <typename SinglePassRange1,
              typename SinglePassRange2,
              typename BinaryPredicate>
    bool equal_r(SinglePassRange1, SinglePassRange2,
                 BinaryPredicate, std::false_type);

    template <typename SinglePassRange1,
              typename SinglePassRange2,
              typename BinaryPredicate>
    bool equal_r(SinglePassRange1 r1, SinglePassRange2 r2,
                 BinaryPredicate p, std::true_type) {
      for (; r1 && r2 && p(r1, r2); r1.drop_front(), r2.drop_front()) {
      }
      return r1.empty() && r2.empty();
    }

  }

  template <typename SinglePassRange1,
            typename SinglePassRange2,
            typename BinaryPredicate>
  bool equal_r(SinglePassRange1 r1, SinglePassRange2 r2,
               BinaryPredicate p) {
    static_assert(support::SinglePassRange<SinglePassRange1>(),
                  "First argument to equal_r must be single pass range.");
    static_assert(support::SinglePassRange<SinglePassRange2>(),
                  "Second argument to equal_r must be single pass range.");
    return detail::equal_r(r1, r2, p,
      support::bool_<support::SinglePassRange<SinglePassRange1>() &&
                     support::SinglePassRange<SinglePassRange2>()>());
  }

  template <typename ReadableSinglePassRange1,
            typename ReadableSinglePassRange2,
            typename BinaryPredicate>
  bool equal(ReadableSinglePassRange1 r1, ReadableSinglePassRange2 r2,
             BinaryPredicate p) {
    static_assert(support::ReadableRange<ReadableSinglePassRange1>(),
                  "First argument to equal must be readable range.");
    static_assert(support::ReadableRange<ReadableSinglePassRange2>(),
                  "Second argument to equal must be readable range.");
    return equal_r(r1, r2, functional::fronts(p));
  }

  template <typename ReadableSinglePassRange1,
            typename ReadableSinglePassRange2>
  bool equal(ReadableSinglePassRange1 r1, ReadableSinglePassRange2 r2) {
    return equal(r1, r2, std::equal_to<>());
  }

}}

#endif
