#ifndef LIBACCENT_RANGES_TAKE_CONDITIONAL_HPP
#define LIBACCENT_RANGES_TAKE_CONDITIONAL_HPP

#include "accent/functional/fronts.hpp"
#include "accent/functional/not.hpp"
#include "accent/support/range.hpp"
#include <boost/compressed_pair.hpp>
#include <cassert>

namespace accent { namespace ranges {

  namespace detail {

    template <typename Derived, typename Inner,
              typename UnaryPredicate, typename Tag>
    class take_until_r_range_impl;

    template <typename Derived, typename Inner, typename UnaryPredicate>
    class take_until_r_range_impl<Derived, Inner, UnaryPredicate,
                                  single_pass_traversal_tag>
        : public support::range_base<Derived> {
      boost::compressed_pair<Inner, UnaryPredicate> data;

    protected:
      const Inner& inner() const { return data.first(); }
      Inner& inner() { return data.first(); }

      const UnaryPredicate& predicate() const { return data.second(); }

      take_until_r_range_impl(Inner inner, UnaryPredicate predicate)
        : data(std::move(inner), std::move(predicate)) {}
      ~take_until_r_range_impl() = default;

    public:
      using value_type = typename Inner::value_type;

      Inner wrapped() const { return inner(); }

      bool empty() const {
        return inner().empty() || predicate()(inner());
      }

      decltype(auto) front() const { return inner().front(); }

      void drop_front() {
        assert(!empty());
        inner().drop_front();
      }
    };

    template <typename Derived, typename Inner, typename UnaryPredicate>
    class take_until_r_range_impl<Derived, Inner, UnaryPredicate,
                                  forward_traversal_tag>
        : public take_until_r_range_impl<Derived, Inner, UnaryPredicate,
                                         single_pass_traversal_tag> {
      using base = take_until_r_range_impl<Derived, Inner, UnaryPredicate,
                                           single_pass_traversal_tag>;
    protected:
      using base::base;
      ~take_until_r_range_impl() = default;

    public:
      using position = support::position_of<Inner>;

      position at_front() const {
        if (this->empty())
          return {};
        return this->inner().at_front();
      }

      void set_front(position p) { this->inner().set_front(p); }
    };
  }

  template <typename Inner, typename UnaryPredicate>
  class take_until_r_range
    : public detail::take_until_r_range_impl<
          take_until_r_range<Inner, UnaryPredicate>,
          Inner, UnaryPredicate,
          support::minimum_tag<support::traversal_of<Inner>,
                               forward_traversal_tag>> {
  public:
    using traversal = support::minimum_tag<support::traversal_of<Inner>,
                                           forward_traversal_tag>;

    take_until_r_range(Inner inner, UnaryPredicate predicate)
      : base(std::move(inner), std::move(predicate)) {}

  private:
    using base = detail::take_until_r_range_impl<
        take_until_r_range<Inner, UnaryPredicate>,
        Inner, UnaryPredicate, traversal>;
  };

  template <typename SinglePassRange, typename UnaryPredicate>
  take_until_r_range<SinglePassRange, UnaryPredicate>
  take_until_r(SinglePassRange r, UnaryPredicate p) {
    static_assert(support::SinglePassRange<SinglePassRange>(),
                  "wrapped range does not model SinglePassRange");
    return { std::move(r), std::move(p) };
  }

  template <typename ReadableSinglePassRange, typename UnaryPredicate>
  auto take_until(ReadableSinglePassRange r, UnaryPredicate p)
      -> decltype(take_until_r(r, functional::fronts(p))) {
    return take_until_r(r, functional::fronts(p));
  }

  template <typename SinglePassRange, typename UnaryPredicate>
  auto take_while_r(SinglePassRange r, UnaryPredicate p)
      -> decltype(take_until_r(r, functional::not_(p))) {
    return take_until_r(r, functional::not_(p));
  }

  template <typename ReadableSinglePassRange, typename UnaryPredicate>
  auto take_while(ReadableSinglePassRange r, UnaryPredicate p)
      -> decltype(take_while_r(r, functional::fronts(p))) {
    return take_while_r(r, functional::fronts(p));
  }

}}

#endif
