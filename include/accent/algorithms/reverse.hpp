#ifndef LIBACCENT_ALGORITHMS_REVERSE_HPP
#define LIBACCENT_ALGORITHMS_REVERSE_HPP

#include "accent/support/algorithm.hpp"

namespace accent { namespace algorithms {

  namespace detail {

    template <typename RearrangeableBidirectionalRange>
    void reverse(RearrangeableBidirectionalRange, std::false_type);

    template <typename RearrangeableBidirectionalRange>
    void reverse(RearrangeableBidirectionalRange r, std::true_type) {
      while (!r.empty()) {
        auto s = r;
        s.drop_front();
        if (!s.empty()) {
          s.drop_back();
          using std::swap;
          swap(r.front(), r.back());
        }
        r = s;
      }
    }

  }

  template <typename RearrangeableBidirectionalRange>
  void reverse(RearrangeableBidirectionalRange r) {
    static_assert(support::BidirectionalRange<
                      RearrangeableBidirectionalRange>(),
                  "reverse requires a bidirectional range");
    static_assert(support::RearrangeableRange<
                      RearrangeableBidirectionalRange>(),
                  "reverse requires a rearrangable range");
    detail::reverse(r,
        support::bool_<support::BidirectionalRange<
                           RearrangeableBidirectionalRange>() &&
                       support::RearrangeableRange<
                           RearrangeableBidirectionalRange>()>());
  }

}}

#endif
