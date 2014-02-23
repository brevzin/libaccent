#ifndef LIBACCENT_ALGORITHMS_ROTATE_HPP
#define LIBACCENT_ALGORITHMS_ROTATE_HPP

#include "accent/functional/operations.hpp"
#include "accent/support/algorithm.hpp"

namespace accent { namespace algorithms {

  namespace detail {

    template <typename RearrangeableForwardRange, typename Tag>
    typename RearrangeableForwardRange::position
    rotate(RearrangeableForwardRange,
           typename RearrangeableForwardRange::position,
           Tag, std::false_type);

    template <typename R1, typename R2>
    void swap_ranges(R1& left, R2& right) {
      while (!left.empty() && !right.empty()) {
        using std::swap;
        swap(left.front(), right.front());
        left.drop_front();
        right.drop_front();
      }
    }

    template <typename Range>
    typename Range::position rotate_fwd(Range r,
                                        typename Range::position new_first) {
      assert(!r.empty() && "Can't call rotate_fwd on empty range.");
      if (r.at_front() == new_first) return new_first;
      bool has_result = false;
      typename Range::position result;
      for (;;) {
        auto left = functional::take_until_position(r, new_first);
        auto right = functional::from(r, new_first);

        detail::swap_ranges(left, right);
        r = left.wrapped();
        if (right.empty()) {
          // rhs shorter or same length
          if (!has_result) {
            has_result = true;
            result = r.at_front();
          }
          if (left.empty()) {
            assert(has_result && "result not assigned");
            return result;
          }
        } else {
          // lhs shorter
          assert(left.empty());
          new_first = right.at_front();
        }
      }
    }

    template <typename RearrangeableForwardRange>
    typename RearrangeableForwardRange::position
    rotate(RearrangeableForwardRange r,
           typename RearrangeableForwardRange::position new_first,
           forward_traversal_tag, std::true_type) {
      return rotate_fwd(r, new_first);
    }

  }

  template <typename RearrangeableForwardRange>
  typename RearrangeableForwardRange::position
  rotate(RearrangeableForwardRange r,
         typename RearrangeableForwardRange::position new_first) {
    static_assert(support::ForwardRange<RearrangeableForwardRange>(),
                  "rotate requires a forward range");
    static_assert(support::RearrangeableRange<RearrangeableForwardRange>(),
                  "rotate requires a rearrangeable range");
    if (r.empty()) return new_first;
    return detail::rotate(r, new_first,
                          support::traversal_of<RearrangeableForwardRange>(),
                          support::bool_<
                              support::ForwardRange<
                                  RearrangeableForwardRange>() &&
                              support::RearrangeableRange<
                                  RearrangeableForwardRange>()
                          >());
  }

}}

#endif
