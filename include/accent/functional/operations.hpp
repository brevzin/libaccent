#ifndef LIBACCENT_FUNCTIONAL_OPERATIONS_HPP
#define LIBACCENT_FUNCTIONAL_OPERATIONS_HPP

#include "accent/ranges/take_conditional.hpp"

namespace accent { namespace functional {

  template <typename SinglePassRange>
  SinglePassRange tail(SinglePassRange r) {
    r.drop_front();
    return r;
  }

  template <typename BidirectionalRange>
  BidirectionalRange init(BidirectionalRange r) {
    r.drop_back();
    return r;
  }

  template <typename ForwardRange>
  ForwardRange from(ForwardRange r, typename ForwardRange::position p) {
    r.set_front(p);
    return r;
  }

  template <typename BidirectionalRange>
  BidirectionalRange until(BidirectionalRange r,
                           typename BidirectionalRange::position p) {
    r.set_back(p);
    return r;
  }

  template <typename ForwardRange>
  class at_position {
    typename ForwardRange::position p;

  public:
    explicit at_position(typename ForwardRange::position p) : p(p) {}

    bool operator ()(const ForwardRange& r) const { return r.at_front() == p; }
  };

  template <typename ForwardRange>
  ranges::take_until_r_range<ForwardRange, at_position<ForwardRange>>
  take_until_position(ForwardRange r, typename ForwardRange::position p) {
    return ranges::take_until_r(r, at_position<ForwardRange>(p));
  }

}}

#endif
