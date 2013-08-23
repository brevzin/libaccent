#ifndef LIBACCENT_RANGES_FILTER_HPP
#define LIBACCENT_RANGES_FILTER_HPP

#include "accent/functional/fronts.hpp"
#include "accent/ranges/reversed.hpp"
#include "accent/support/range.hpp"
#include <boost/compressed_pair.hpp>
#include <cassert>

namespace accent { namespace ranges {

  namespace detail {

    template <typename Derived, typename Inner,
              typename UnaryPredicate, typename Tag>
    class filter_r_range_impl;

    template <typename Derived, typename Inner, typename UnaryPredicate>
    class filter_r_range_impl<Derived, Inner, UnaryPredicate,
                              single_pass_traversal_tag>
        : public support::range_base<Derived> {
      boost::compressed_pair<Inner, UnaryPredicate> data;

    protected:
      const Inner& inner() const { return data.first(); }
      Inner& inner() { return data.first(); }

      const UnaryPredicate& predicate() const { return data.second(); }

      filter_r_range_impl(Inner inner, UnaryPredicate predicate)
          : data(std::move(inner), std::move(predicate)) {
        adjust_front();
      }
      ~filter_r_range_impl() = default;

    public:
      using value_type = typename Inner::value_type;

      bool empty() const {
        return inner().empty();
      }

      auto front() const -> decltype(inner().front()) {
        return inner().front();
      }

      void drop_front() {
        inner().drop_front();
        adjust_front();
      }

    private:
      void adjust_front() {
        while (inner() && !predicate()(support::const_(inner()))) {
          inner().drop_front();
        }
      }
    };

    template <typename Derived, typename Inner, typename UnaryPredicate>
    class filter_r_range_impl<Derived, Inner, UnaryPredicate,
                              forward_traversal_tag>
        : public filter_r_range_impl<Derived, Inner, UnaryPredicate,
                                     single_pass_traversal_tag> {
      using base = filter_r_range_impl<Derived, Inner, UnaryPredicate,
                                       single_pass_traversal_tag>;

    protected:
      using base::base;
      ~filter_r_range_impl() = default;

    public:
      using position = support::position_of<Inner>;

      position at_front() const { return this->inner().at_front(); }

      void set_front(position p) { this->inner().set_front(p); }
    };

    template <typename Derived, typename Inner, typename UnaryPredicate>
    class filter_r_range_impl<Derived, Inner, UnaryPredicate,
                              bidirectional_traversal_tag>
        : public filter_r_range_impl<Derived, Inner, UnaryPredicate,
                                     forward_traversal_tag> {
      using base = filter_r_range_impl<Derived, Inner, UnaryPredicate,
                                       forward_traversal_tag>;

    protected:
      filter_r_range_impl(Inner inner, UnaryPredicate predicate)
          : base(std::move(inner), std::move(predicate)) {
        adjust_back();
      }
      ~filter_r_range_impl() = default;

    public:
      using position = typename base::position;

      auto back() const -> decltype(this->inner().back()) {
        return this->inner().back();
      }

      void drop_back() {
        this->inner().drop_back();
        adjust_back();
      }

      position at_back() const { return this->inner().at_back(); }

      void set_back(position p) { this->inner().set_back(p); }

    private:
      void adjust_back() {
        while (this->inner() && !this->predicate()(reversed(this->inner()))) {
          this->inner().drop_back();
        }
      }
    };

  }

  template <typename Inner, typename UnaryPredicate>
  class filter_r_range
      : public detail::filter_r_range_impl<
            filter_r_range<Inner, UnaryPredicate>,
            Inner, UnaryPredicate,
            support::minimum_tag<support::traversal_of<Inner>,
                                 bidirectional_traversal_tag>> {
  public:
    static_assert(support::SinglePassRange<Inner>(),
                  "filter requires a single pass range");

    using traversal = support::minimum_tag<support::traversal_of<Inner>,
                                           bidirectional_traversal_tag>;

    filter_r_range(Inner inner, UnaryPredicate predicate)
      : base(std::move(inner), std::move(predicate)) {}

  private:
    using base = detail::filter_r_range_impl<
        filter_r_range<Inner, UnaryPredicate>,
        Inner, UnaryPredicate, traversal>;
  };

  template <typename SinglePassRange, typename UnaryPredicate>
  filter_r_range<SinglePassRange, UnaryPredicate>
  filter_r(SinglePassRange r, UnaryPredicate p) {
    return { std::move(r), std::move(p) };
  }

  template <typename ReadableSinglePassRange, typename UnaryPredicate>
  auto filter(ReadableSinglePassRange r, UnaryPredicate p)
      -> decltype(filter_r(r, functional::fronts(p))) {
    static_assert(support::ReadableRange<ReadableSinglePassRange>(),
                  "filter requires a readable range");
    return filter_r(r, functional::fronts(p));
  }

}}

#endif
