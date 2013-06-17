#ifndef LIBACCENT_RANGES_AS_HPP
#define LIBACCENT_RANGES_AS_HPP

#include "accent/support/concepts.hpp"
#include "accent/support/range_base.hpp"
#include "accent/support/tag_functions.hpp"

namespace accent { namespace ranges {

  namespace detail {

    template <typename Inner>
    class as_single_pass_range :
        public support::range_base<as_single_pass_range<Inner>> {
      Inner inner;

    public:
      using value_type = typename Inner::value_type;
      using traversal = single_pass_traversal_tag;

      static_assert(support::traversal_supports<traversal, Inner>::value,
          "Cannot use as_single_pass to increase traveral support.");

      explicit as_single_pass_range(Inner inner) : inner(std::move(inner)) {}

      bool empty() const { return inner.empty(); }
      void drop_front() { inner.drop_front(); }
      auto front() const -> decltype(inner.front()) { return inner.front(); }
    };

    template <typename Inner>
    class as_forward_range :
        public support::range_base<as_forward_range<Inner>> {
      Inner inner;

    public:
      using value_type = typename Inner::value_type;
      using traversal = forward_traversal_tag;
      using position = support::position_of<Inner>;

      static_assert(support::traversal_supports<traversal, Inner>::value,
          "Cannot use as_forward to increase traveral support.");

      explicit as_forward_range(Inner inner) : inner(std::move(inner)) {}

      bool empty() const { return inner.empty(); }
      void drop_front() { inner.drop_front(); }
      auto front() const -> decltype(inner.front()) { return inner.front(); }

      auto at_front() const -> decltype(inner.at_front()) {
        return inner.at_front();
      }
      as_forward_range from(position p) const {
        return as_forward_range(inner.from(p));
      }
      auto until(position p) const
          -> as_forward_range<decltype(inner.until(p))> {
        return as_forward_range<decltype(inner.until(p))>(inner.until(p));
      }
    };

  }

  template <typename Inner>
  detail::as_single_pass_range<Inner> as_single_pass(Inner inner) {
    return detail::as_single_pass_range<Inner>(std::move(inner));
  }

  template <typename Inner>
  detail::as_forward_range<Inner> as_forward(Inner inner) {
    return detail::as_forward_range<Inner>(std::move(inner));
  }

}}

#endif
