#ifndef LIBACCENT_RANGES_PROJECT_HPP
#define LIBACCENT_RANGES_PROJECT_HPP

#include "accent/support/range.hpp"
#include <boost/compressed_pair.hpp>
#include <boost/optional.hpp>
#include <type_traits>
#include <utility>

namespace accent { namespace ranges {

  template <typename Inner, typename Projection>
  class projected_range;

  template <typename Inner, typename Projection>
  class projected_position {
    Inner inner_position;
    // Needed to ensure default-constructability. Should optimize for when
    // the projection is default-constructible anyway.
    boost::optional<Projection> projection;

  public:
    using value_type =
        typename std::decay<decltype(std::declval<Projection>()(
                                         *std::declval<Inner>()))
                           >::type;

    projected_position() = default;

    projected_position(Inner inner, Projection projection)
      : inner_position(std::move(inner)), projection(std::move(projection)) {}

    Inner inner() const { return inner_position; }

    bool valid() const { return inner().valid(); }
    explicit operator bool() const { return valid(); }

    decltype(auto) operator *() const { return (*projection)(*inner()); }
  };
  template <typename Inner, typename Projection>
  bool operator ==(const projected_position<Inner, Projection>& l,
                   const projected_position<Inner, Projection>& r) {
    // Must assume that functions are equal; there is no way to compare them.
    return l.inner() == r.inner();
  }
  template <typename Inner, typename Projection>
  bool operator !=(const projected_position<Inner, Projection>& l,
                   const projected_position<Inner, Projection>& r) {
    return !(l == r);
  }

  namespace detail {

    template <typename Derived, typename Inner,
              typename Projection, typename Tag>
    class projected_range_impl;

    template <typename Derived, typename Inner, typename Projection>
    class projected_range_impl<Derived, Inner, Projection,
                               single_pass_traversal_tag>
        : public support::range_base<Derived> {
      boost::compressed_pair<Inner, Projection> data;

    protected:
      const Inner& inner() const { return data.first(); }
      Inner& inner() { return data.first(); }

      const Projection& projection() const { return data.second(); }

      projected_range_impl(Inner inner, Projection projection)
        : data(std::move(inner), std::move(projection)) {}
      ~projected_range_impl() = default;

    public:
      using value_type =
          typename std::decay<decltype(std::declval<Projection>()(
                                           std::declval<Inner>().front()))
                             >::type;
      using traversal = support::traversal_of<Inner>;

      bool empty() const { return inner().empty(); }

      void drop_front() { inner().drop_front(); }

      decltype(auto) front() const { return projection()(inner().front()); }
    };

    template <typename Derived, typename Inner, typename Projection>
    class projected_range_impl<Derived, Inner, Projection,
                               forward_traversal_tag>
        : public projected_range_impl<Derived, Inner, Projection,
                                      single_pass_traversal_tag> {
      using base = projected_range_impl<Derived, Inner, Projection,
                                        single_pass_traversal_tag>;
    protected:
      using base::base;
      ~projected_range_impl() = default;

    public:
      using position = projected_position<support::position_of<Inner>,
                                          Projection>;

      position at_front() const {
        return { this->inner().at_front(), this->projection() };
      }
      void set_front(position p) { this->inner().set_front(p.inner()); }
    };

    template <typename Derived, typename Inner, typename Projection>
    class projected_range_impl<Derived, Inner, Projection,
                               bidirectional_traversal_tag>
        : public projected_range_impl<Derived, Inner, Projection,
                                      forward_traversal_tag> {
      using base = projected_range_impl<Derived, Inner, Projection,
                                        forward_traversal_tag>;
    protected:
      using base::base;
      ~projected_range_impl() = default;

    public:
      using position = typename base::position;

      decltype(auto) back() const {
        return this->projection()(this->inner().back());
      }
      void drop_back() { this->inner().drop_back(); }
      position at_back() const {
        return { this->inner().at_back(), this->projection() };
      }
      void set_back(position p) { this->inner().set_back(p.inner()); }
    };

    template <typename Derived, typename Inner, typename Projection>
    class projected_range_impl<Derived, Inner, Projection,
                               random_access_traversal_tag>
        : public projected_range_impl<Derived, Inner, Projection,
                                      bidirectional_traversal_tag> {
      using base = projected_range_impl<Derived, Inner, Projection,
                                        bidirectional_traversal_tag>;
    protected:
      using base::base;
      ~projected_range_impl() = default;
    };

  }

  template <typename Inner, typename Projection>
  class projected_range
    : public detail::projected_range_impl<projected_range<Inner, Projection>,
                                          Inner, Projection,
                                          support::traversal_of<Inner>>
  {
    using base = detail::projected_range_impl<projected_range<Inner,Projection>,
                                              Inner, Projection,
                                              support::traversal_of<Inner>>;

  public:
    projected_range(Inner inner, Projection projection)
      : base(std::move(inner), std::move(projection)) {}
  };

  template <typename Inner, typename RegularUnaryFunction>
  projected_range<Inner, RegularUnaryFunction>
  project(Inner i, RegularUnaryFunction p) {
    static_assert(support::SinglePassRange<Inner>(),
                  "wrapped range does not model SinglePassRange");
    return { std::move(i), std::move(p) };
  }

}}

#endif
