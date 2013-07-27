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
      void set_front(position p) { inner.set_front(p); }
    };

    template <typename Inner>
    class as_bidirectional_range :
        public support::range_base<as_bidirectional_range<Inner>> {
      Inner inner;

    public:
      using value_type = typename Inner::value_type;
      using traversal = bidirectional_traversal_tag;
      using position = support::position_of<Inner>;

      static_assert(support::traversal_supports<traversal, Inner>::value,
          "Cannot use as_bidirectional to increase traveral support.");

      explicit as_bidirectional_range(Inner inner) : inner(std::move(inner)) {}

      bool empty() const { return inner.empty(); }
      void drop_front() { inner.drop_front(); }
      auto front() const -> decltype(inner.front()) { return inner.front(); }

      auto at_front() const -> decltype(inner.at_front()) {
        return inner.at_front();
      }
      void set_front(position p) { inner.set_front(p); }

      auto back() const -> decltype(inner.back()) { return inner.back(); }
      void drop_back() { inner.drop_back(); }
      auto at_back() const -> decltype(inner.at_back()) {
        return inner.at_back();
      }
      void set_back(position p) { inner.set_back(p); }
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

  template <typename Inner>
  detail::as_bidirectional_range<Inner> as_bidirectional(Inner inner) {
    return detail::as_bidirectional_range<Inner>(std::move(inner));
  }

}}

#endif
