#ifndef LIBACCENT_RANGES_REVERSED_HPP
#define LIBACCENT_RANGES_REVERSED_HPP

#include "accent/support/range.hpp"
#include <utility>

namespace accent { namespace ranges {

  namespace detail {

    template <typename Derived, typename Inner, typename Tag>
    class reversed_range_impl;

    template <typename Derived, typename Inner>
    class reversed_range_impl<Derived, Inner, bidirectional_traversal_tag>
        : public support::range_base<Derived> {
    protected:
      Inner inner;

      explicit reversed_range_impl(Inner inner) : inner(std::move(inner)) {}

    public:
      bool empty() const { return inner.empty(); }

      auto front() const -> decltype(inner.back()) { return inner.back(); }
      auto back() const -> decltype(inner.front()) { return inner.front(); }

      void drop_front() { inner.drop_back(); }
      void drop_back() { inner.drop_front(); }

      using position = support::position_of<Inner>;

      position at_front() const { return inner.at_back(); }
      position at_back() const { return inner.at_front(); }

      void set_front(position p) { inner.set_back(p); }
      void set_back(position p) { inner.set_front(p); }
    };

  }

  template <typename Inner>
  class reversed_range
      : public detail::reversed_range_impl<reversed_range<Inner>, Inner,
                                           support::traversal_of<Inner>> {
    using base = detail::reversed_range_impl<reversed_range, Inner,
                                             support::traversal_of<Inner>>;

  public:
    static_assert(support::BidirectionalRange<Inner>(),
                  "reversed requires a bidirectional range");

    using traversal = support::traversal_of<Inner>;

    explicit reversed_range(Inner inner) : base(std::move(inner)) {}
  };

  template <typename BidirectionalRange>
  reversed_range<BidirectionalRange> reversed(BidirectionalRange r) {
    return reversed_range<BidirectionalRange>{ std::move(r) };
  }

}}

#endif
