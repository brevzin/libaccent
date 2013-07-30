#ifndef LIBACCENT_RANGES_REVERSED_HPP
#define LIBACCENT_RANGES_REVERSED_HPP

#include <utility>

namespace accent { namespace ranges {

  template <typename Inner>
  class reversed_range {
    Inner inner;

  public:
    explicit reversed_range(Inner inner) : inner(std::move(inner)) {}

    auto front() const -> decltype(inner.back()) { return inner.back(); }
  };

  template <typename BidirectionalRange>
  reversed_range<BidirectionalRange> reversed(BidirectionalRange r) {
    return reversed_range<BidirectionalRange>{ std::move(r) };
  }

}}

#endif
