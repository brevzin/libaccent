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
      decltype(auto) operator ()(const Range& r) const { return r.front(); }
    };
    struct drop_front_op {
      template <typename Range>
      void operator ()(Range& r) const { r.drop_front(); }
    };

    template <typename Derived, typename... Inners>
    class concat_range_impl<Derived, single_pass_traversal_tag, Inners...>
        : public support::range_base<Derived> {
      int last_index() const { return sizeof...(Inners) - 1; }

    protected:
      using tuple = std::tuple<Inners...>;
      using first = support::head<Inners...>;
      using value_ref = decltype(std::declval<first>().front());

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
      using value_type = typename first::value_type;
      static_assert(support::all<std::is_same<value_type,
                                              typename Inners::value_type
                                             >::value...>::value,
                    "all concatenated ranges must have the same value type");

      bool empty() const {
        return current > this->self().last_index();
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

    template <typename... Positions>
    class concat_position {
      using first = support::head<Positions...>;
      using value_ref = decltype(*std::declval<first>());
      using invalid = invalid_position<value_ref>;
      using variant = support::variant<Positions..., invalid>;
      variant data;

    public:
      using value_type = typename std::decay<value_ref>::type;

      concat_position() noexcept : data(invalid{}) {}
      template <typename T>
      explicit concat_position(T t, int index) : data(t, index) {}

      template <typename Position>
      Position get() const { return data.template get<Position>(); }
      template <typename Operation>
      decltype(auto) apply(Operation op) const { return data.apply(op); }
      int which() const { return data.which(); }

      bool valid() const { return apply(valid_op()); }
      explicit operator bool() const { return valid(); }
      value_ref operator *() const { return apply(deref_op{}); }
    };

    template <typename... Positions>
    bool operator ==(const concat_position<Positions...>& l,
                     const concat_position<Positions...>& r) {
      if (l.which() != r.which()) {
        return false;
      }
      return l.apply(make_equal_op(r));
    }

    template <typename... Positions>
    bool operator !=(const concat_position<Positions...>& l,
                     const concat_position<Positions...>& r) {
      return !(l == r);
    }

    template <typename Position>
    class at_front_op {
      int current;

    public:
      explicit at_front_op(int current) : current(current) {}

      template <typename Range>
      Position operator ()(const Range& r) const {
        return Position{r.at_front(), current};
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
      concat_range_impl(Inners... inners) : base(inners...) {}
      ~concat_range_impl() = default;

    public:
      using position = concat_position<typename Inners::position...>;

      position at_front() const {
        return this->empty() ?
            position() : this->apply(at_front_op<position>{this->current});
      }

      void set_front(position p) {
        this->current = p.which();
        if (!this->empty()) {
          this->apply(make_set_front_op(p));
        }
      }
    };

    struct back_op {
      template <typename Range>
      decltype(auto) operator ()(const Range& r) const { return r.back(); }
    };

    struct drop_back_op {
      template <typename Range>
      void operator ()(Range& r) const { r.drop_back(); }
    };

    template <typename Position>
    class at_back_op {
      int current;

    public:
      explicit at_back_op(int current) : current(current) {}

      template <typename Range>
      Position operator ()(const Range& r) const {
        return Position{r.at_back(), current};
      }
    };

    template <typename Position>
    class set_back_op {
      Position p;

    public:
      explicit set_back_op(Position p) : p(p) {}

      template <typename Range>
      void operator ()(Range& r) const {
        r.set_back(p.template get<typename Range::position>());
      }
    };

    template <typename Position>
    set_back_op<Position> make_set_back_op(Position p) {
      return set_back_op<Position>(p);
    }

    template <typename Derived, typename... Inners>
    class concat_range_impl<Derived, bidirectional_traversal_tag, Inners...>
        : public concat_range_impl<Derived, forward_traversal_tag, Inners...> {
      using base = concat_range_impl<Derived, forward_traversal_tag, Inners...>;

      friend class concat_range_impl<Derived, single_pass_traversal_tag,
                                     Inners...>;
      int last_index() const { return current_back; }

    protected:
      int current_back = sizeof...(Inners) - 1;

      concat_range_impl(Inners... inners)
          : base(inners...) {
        skip_empty_back();
      }
      ~concat_range_impl() = default;

      template <typename Operation>
      decltype(auto) apply_back(Operation op) {
        assert(!this->empty() && "operation on empty range");
        return support::dispatch(this->inners, op, current_back);
      }
      template <typename Operation>
      decltype(auto) apply_back(Operation op) const {
        assert(!this->empty() && "operation on empty range");
        return support::dispatch(this->inners, op, current_back);
      }

      using value_ref = typename base::value_ref;

    public:
      using position = typename base::position;

      value_ref back() const {
        return apply_back(back_op{});
      }

      void drop_back() {
        apply_back(drop_back_op{});
        skip_empty_back();
      }


      position at_back() const {
        return this->empty() ?
            position() : this->apply_back(at_back_op<position>{current_back});
      }

      void set_back(position p) {
        current_back = p.which();
        if (current_back == sizeof...(Inners)) {
          current_back = -1;
        }
        if (!this->empty()) {
          this->apply_back(make_set_back_op(p));
        }
      }

    private:
      void skip_empty_back() {
        while (!this->empty() && apply_back(empty_op{})){
          --current_back;
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

    explicit concat_range(Inners... inners) : base(inners...) {}

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
