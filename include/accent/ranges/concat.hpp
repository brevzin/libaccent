#ifndef LIBACCENT_RANGES_CONCAT_HPP
#define LIBACCENT_RANGES_CONCAT_HPP

#include "accent/support/dispatch.hpp"
#include "accent/support/range.hpp"
#include "accent/support/variant.hpp"
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <tuple>
#include <type_traits>
#include <utility>

namespace accent { namespace ranges {

  namespace detail {

    template <typename Derived, typename Tag, typename... Inners>
    class concat_range_impl;

    struct empty_op {
      template <typename Range>
      bool operator ()(const Range& r) const { return r.empty(); }
    };
    struct front_op {
      template <typename Range>
      auto operator ()(const Range& r) const -> decltype(r.front()) {
        return r.front();
      }
    };
    struct drop_front_op {
      template <typename Range>
      void operator ()(Range& r) const { r.drop_front(); }
    };

    template <typename Derived, typename... Inners>
    class concat_range_impl<Derived, single_pass_traversal_tag, Inners...>
        : public support::range_base<Derived> {
    protected:
      using tuple = std::tuple<Inners...>;
      using value_ref = decltype(std::declval<support::head<Inners...>>()
                                     .front());

      tuple inners;
      int current = 0;

      concat_range_impl(Inners... inners) : inners(inners...) { skip_empty(); }
      ~concat_range_impl() = default;

      template <typename Operation>
      decltype(auto) apply(Operation op) {
        assert(!empty() && "operation on empty range");
        return support::dispatch(inners, op, current);
      }
      template <typename Operation>
      decltype(auto) apply(Operation op) const {
        assert(!empty() && "operation on empty range");
        return support::dispatch(inners, op, current);
      }

    public:
      using value_type = typename support::head<Inners...>::value_type;
      static_assert(support::all<std::is_same<value_type,
                                              typename Inners::value_type
                                             >::value...>::value,
                    "all concatenated ranges must have the same value type");

      bool empty() const {
        return current >= sizeof...(Inners);
      }

      value_ref front() const {
        return apply(front_op{});
      }

      void drop_front() {
        apply(drop_front_op{});
        skip_empty();
      }

    private:
      void skip_empty() {
        while (!empty() && apply(empty_op{})){
          ++current;
        }
      }
    };

    template <typename ValueRef>
    struct invalid_position {
      using value_type = typename std::decay<ValueRef>::type;

      bool valid() const { return false; }
      explicit operator bool() const { return false; }

      ValueRef operator *() const { std::abort(); }
    };
    template <typename ValueRef>
    bool operator ==(invalid_position<ValueRef> lhs,
                     invalid_position<ValueRef> rhs) {
      return true;
    }

    struct valid_op {
      template <typename Position>
      bool operator ()(const Position& p) const { return p.valid(); }
    };

    struct deref_op {
      template <typename Position>
      decltype(auto) operator ()(const Position& p) const { return *p; }
    };

    template <typename Position>
    class equal_op {
      Position p;

    public:
      explicit equal_op(const Position& p) : p(p) {}

      template <typename RealPosition>
      bool operator ()(const RealPosition& lhs) const {
        return lhs == p.template get<RealPosition>();
      }
    };

    template <typename Position>
    equal_op<Position> make_equal_op(const Position& p) {
      return equal_op<Position>{p};
    }

    template <typename... Ranges>
    class concat_position {
      using value_ref = decltype(*std::declval<
                            typename support::head<Ranges...>::position>());
      using invalid = invalid_position<value_ref>;
      using variant = support::variant<typename Ranges::position...,
                                       invalid>;
      variant data;

    public:
      using value_type = typename std::decay<value_ref>::type;

      concat_position() noexcept : data(invalid{}) {}
      template <typename T>
      explicit concat_position(T t) : data(t) {}

      template <typename Position>
      Position get() const { return data.template get<Position>(); }
      template <typename Operation>
      decltype(auto) apply(Operation op) const { return data.apply(op); }
      int which() const { return data.which(); }

      bool valid() const { return apply(valid_op()); }
      explicit operator bool() const { return valid(); }
      value_ref operator *() const { return apply(deref_op{}); }
    };

    template <typename... Ranges>
    bool operator ==(const concat_position<Ranges...>& l,
                     const concat_position<Ranges...>& r) {
      if (l.which() != r.which()) {
        return false;
      }
      return l.apply(make_equal_op(r));
    }

    template <typename... Ranges>
    bool operator !=(const concat_position<Ranges...>& l,
                     const concat_position<Ranges...>& r) {
      return !(l == r);
    }

    template <typename Position>
    struct at_front_op {
      template <typename Range>
      Position operator ()(const Range& r) const {
        return Position{r.at_front()};
      }
    };

    template <typename Position>
    class set_front_op {
      Position p;

    public:
      explicit set_front_op(Position p) : p(p) {}

      template <typename Range>
      void operator ()(Range& r) const {
        r.set_front(p.template get<typename Range::position>());
      }
    };

    template <typename Position>
    set_front_op<Position> make_set_front_op(Position p) {
      return set_front_op<Position>(p);
    }

    template <typename Derived, typename... Inners>
    class concat_range_impl<Derived, forward_traversal_tag, Inners...>
        : public concat_range_impl<Derived, single_pass_traversal_tag,
                                   Inners...> {
      using base = concat_range_impl<Derived, single_pass_traversal_tag,
                                     Inners...>;
    protected:
      using tuple = typename base::tuple;

      concat_range_impl(Inners... inners) : base(inners...) {}
      ~concat_range_impl() = default;

    public:
      using position = concat_position<Inners...>;

      position at_front() const {
        return this->empty() ? position()
                             : this->apply(at_front_op<position>{});
      }

      void set_front(position p) {
        this->current = p.which();
        if (!this->empty()) {
          this->apply(make_set_front_op(p));
        }
      }
    };

  }

  template <typename... Inners>
  class concat_range
      : public detail::concat_range_impl<
          concat_range<Inners...>,
          support::minimum_tag<support::traversal_of<Inners>...>,
          Inners...> {
  public:
    using traversal = support::minimum_tag<support::traversal_of<Inners>...>;

    concat_range(Inners... inners) : base(inners...) {}

  private:
    using base = detail::concat_range_impl<concat_range, traversal, Inners...>;
  };

  template <typename... SinglePassRanges>
  concat_range<SinglePassRanges...> concat(SinglePassRanges... ranges) {
    return concat_range<SinglePassRanges...>(ranges...);
  }

  // Must have at least one range.
  void concat() = delete;

}}

#endif
