#ifndef LIBACCENT_RANGES_ZIP_HPP
#define LIBACCENT_RANGES_ZIP_HPP

#include "accent/support/range.hpp"
#include <tuple>

namespace accent { namespace ranges {

  namespace detail_zip {

    inline bool any_merge(bool b) {
      return b;
    }

    template <typename... B>
    bool any_merge(bool b, B... bs) {
      return b || any_merge(bs...);
    }

    template <typename Tuple, typename Op, std::size_t... Indices>
    bool any_impl(Tuple& t, Op op, std::index_sequence<Indices...>) {
      return any_merge(op(std::get<Indices>(t))...);
    }

    template <typename Tuple, typename Op, std::size_t... Indices>
    auto map_impl(Tuple& t, Op op, std::index_sequence<Indices...>) {
      return std::forward_as_tuple(op(std::get<Indices>(t))...);
    }

    template <typename... T> void ignore(T...) {}

    template <typename Tuple, typename Op, std::size_t... Indices>
    void apply_impl(Tuple& t, Op op, std::index_sequence<Indices...>) {
      ignore((op(std::get<Indices>(t)), 0)...);
    }

    struct empty_op {
      template <typename Range>
      bool operator ()(const Range& r) const { return r.empty(); }
    };

    struct front_op {
      template <typename Range>
      decltype(auto) operator ()(const Range& r) const { return r.front(); }
    };

    struct drop_front_op {
      template <typename Range>
      void operator ()(Range& r) const { r.drop_front(); }
    };

    template <typename Derived, typename Tag, typename... Inners>
    class zip_shortest_range_impl;

    template <typename Derived, typename... Inners>
    class zip_shortest_range_impl<Derived, single_pass_traversal_tag, Inners...>
        : public support::range_base<Derived> {
    protected:
      using tuple = std::tuple<Inners...>;
      using value_ref = std::tuple<decltype(std::declval<Inners>().front())...>;

      tuple inners;

      zip_shortest_range_impl(Inners... inners) : inners(inners...) {}

      template <typename Op>
      bool any(Op op) const {
        return any_impl(inners, op, std::index_sequence_for<Inners...>());
      }

      template <typename Op>
      auto map(Op op) const {
        return map_impl(inners, op, std::index_sequence_for<Inners...>());
      }

      template <typename Op>
      void apply(Op op) {
        return apply_impl(inners, op, std::index_sequence_for<Inners...>());
      }

    public:
      using value_type = std::tuple<typename Inners::value_type...>;

      bool empty() const {
        return any(empty_op{});
      }

      value_ref front() const {
        return map(front_op{});
      }

      void drop_front() {
        apply(drop_front_op{});
      }
    };

  }

  template <typename... Inners>
  class zip_shortest_range
      : public detail_zip::zip_shortest_range_impl<
          zip_shortest_range<Inners...>,
          support::minimum_tag<support::traversal_of<Inners>...>,
          Inners...> {
  public:
    using traversal = support::minimum_tag<support::traversal_of<Inners>...>;

    explicit zip_shortest_range(Inners... inners) : base(inners...) {}

  private:
    using base = detail_zip::zip_shortest_range_impl<
        zip_shortest_range, traversal, Inners...>;
  };

  template <typename... SinglePassRanges>
  zip_shortest_range<SinglePassRanges...>
  zip_shortest(SinglePassRanges... ranges) {
    return zip_shortest_range<SinglePassRanges...>(ranges...);
  }

  // Must have at least one range.
  void zip_shortest() = delete;

}}

#endif
