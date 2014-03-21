#ifndef LIBACCENT_RANGES_ZIP_HPP
#define LIBACCENT_RANGES_ZIP_HPP

#include "accent/support/functors.hpp"
#include "accent/support/range.hpp"
#include "accent/support/tuple.hpp"
#include <functional>
#include <tuple>

namespace accent { namespace ranges {

  namespace detail_zip {

    struct swap_op {
      template <typename T, typename U>
      void operator ()(std::tuple<T&, U&> tu) const {
        using std::swap;
        swap(std::get<0>(tu), std::get<1>(tu));
      }
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
        return support::reduce_tuple(std::logical_or<>(),
                                     support::map_tuple(op, inners));
      }

      template <typename Op>
      auto map(Op op) const {
        return support::map_tuple(op, inners);
      }

      template <typename Op>
      void apply(Op op) {
        support::for_tuple(op, inners);
      }

    public:
      using value_type = std::tuple<typename Inners::value_type...>;

      bool empty() const {
        return any(support::functors::empty_op{});
      }

      value_ref front() const {
        return map(support::functors::front_op{});
      }

      void drop_front() {
        apply(support::functors::drop_front_op{});
      }
    };

    struct equal_op {
      template <typename P>
      bool operator ()(const std::tuple<P, P>& ps) const {
        return std::get<0>(ps) == std::get<1>(ps);
      }
    };

    template <typename... Inners>
    class zip_position {
      using positions = std::tuple<Inners...>;
      using value_ref = std::tuple<decltype(*std::declval<Inners>())...>;
      positions inners;

      template <typename Op>
      bool all(Op op) const {
        return support::reduce_tuple(std::logical_and<>(),
                                     support::map_tuple(op, inners));
      }

      template <typename Op>
      auto map(Op op) const {
        return support::map_tuple(op, inners);
      }

      template <typename D, typename T, typename... I>
      friend class zip_shortest_range_impl;

    public:
      using value_type = std::tuple<typename Inners::value_type...>;

      zip_position() noexcept = default;
      zip_position(positions inners) : inners(inners) {}
      bool valid() const {
        return all(support::functors::valid_op{});
      }
      explicit operator bool() const { return valid(); }
      value_ref operator *() const {
        return map(support::functors::deref_op{});
      }

      friend bool operator ==(const zip_position& l, const zip_position& r) {
        return support::reduce_tuple(std::logical_and<>(),
            support::map_tuple(equal_op{},
                               support::zip_tuples(l.inners, r.inners)));
      }
    };

    template <typename... Inners>
    bool operator !=(const zip_position<Inners...>& l,
                     const zip_position<Inners...>& r) {
      return !(l == r);
    }

    struct at_front_op {
      template <typename T>
      auto operator ()(const T& t) const { return t.at_front(); }
    };

    struct set_front_op {
      template <typename R, typename P>
      void operator ()(const std::tuple<R&, P&>& rp) const {
        std::get<0>(rp).set_front(std::get<1>(rp));
      }
    };

    template <typename Derived, typename... Inners>
    class zip_shortest_range_impl<Derived, forward_traversal_tag, Inners...>
        : public zip_shortest_range_impl<Derived, single_pass_traversal_tag,
                                         Inners...> {
      using base = zip_shortest_range_impl<Derived, single_pass_traversal_tag,
                                           Inners...>;
    protected:
      zip_shortest_range_impl(Inners... inners) : base(inners...) {}

    public:
      using position = zip_position<typename Inners::position...>;

      position at_front() const {
        return position(this->map(at_front_op{}));
      }

      void set_front(position p) {
        support::for_tuple(set_front_op{},
                           support::zip_tuples(this->inners, p.inners));
      }
    };

  }

  template <typename... Inners>
  class zip_shortest_range
      : public detail_zip::zip_shortest_range_impl<
          zip_shortest_range<Inners...>,
          support::minimum_tag<forward_traversal_tag,
                               support::traversal_of<Inners>...>,
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

// Overload swap for tuples of references. Unfortunately, this has to happen
// in namespace std, without a user-defined type. This is technically undefined.
namespace std {

  template <typename... Ts>
  void swap(tuple<Ts&...> t1, tuple<Ts&...> t2) {
    ::accent::support::for_tuple(
        ::accent::ranges::detail_zip::swap_op{},
        ::accent::support::zip_tuples(t1, t2));
  }

  template <typename... Ts, typename... Ys>
  void swap(tuple<Ts...>& t1, tuple<Ys&...> t2) {
    static_assert(sizeof...(Ts) == sizeof...(Ys), "Tuples must be same size.");
    ::accent::support::for_tuple(
        ::accent::ranges::detail_zip::swap_op{},
        ::accent::support::zip_tuples(t1, t2));
  }

  template <typename... Ts, typename... Ys>
  void swap(tuple<Ts&...> t1, tuple<Ys...>& t2) {
    static_assert(sizeof...(Ts) == sizeof...(Ys), "Tuples must be same size.");
    ::accent::support::for_tuple(
        ::accent::ranges::detail_zip::swap_op{},
        ::accent::support::zip_tuples(t1, t2));
  }

}

#endif
