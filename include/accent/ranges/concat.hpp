#ifndef LIBACCENT_RANGES_CONCAT_HPP
#define LIBACCENT_RANGES_CONCAT_HPP

#include "accent/support/range.hpp"
#include <cassert>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace accent { namespace ranges {

  namespace detail {

    template <typename T, typename... Ts>
    using head = T;

    template <bool... Bs> struct all;
    template <> struct all<> : support::bool_<true> {};
    template <bool B, bool... Bs> struct all<B, Bs...>
        : support::bool_<B && all<Bs...>::value> {};

    template <typename Indices, typename... Ranges>
    class sp_ops;

    template <std::size_t... Indices, typename... Ranges>
    class sp_ops<std::index_sequence<Indices...>, Ranges...> {
      using tuple = std::tuple<Ranges...>;
      using value_ref = decltype(std::declval<head<Ranges...>>().front());
      static_assert(all<std::is_same<value_ref,
                                     decltype(std::declval<Ranges>().front())
                                    >::value...>::value,
                "all concatenated ranges must have the same front return type");

      template <std::size_t I>
      static bool empty_impl(const tuple& t) { return std::get<I>(t).empty(); }

      template <std::size_t I>
      static value_ref front_impl(const tuple& t) {
        return std::get<I>(t).front();
      }

      template <std::size_t I>
      static void drop_front_impl(tuple& t) { std::get<I>(t).drop_front(); }

      struct ops {
        bool (*empty)(const tuple&);
        value_ref (*front)(const tuple&);
        void (*drop_front)(tuple&);
      };
      static constexpr ops table[] = {
        {
          &empty_impl<Indices>,
          &front_impl<Indices>,
          &drop_front_impl<Indices>
        }...
      };

    public:
      constexpr const ops& operator [](int i) const { return table[i]; }
    };

    template <std::size_t... Indices, typename... Ranges>
    constexpr typename sp_ops<std::index_sequence<Indices...>, Ranges...>::ops
    sp_ops<std::index_sequence<Indices...>, Ranges...>::table[];

    template <typename Derived, typename Tag, typename... Inners>
    class concat_range_impl;

    template <typename Derived, typename... Inners>
    class concat_range_impl<Derived, single_pass_traversal_tag, Inners...>
        : public support::range_base<Derived> {
    protected:
      using tuple = std::tuple<Inners...>;
      using value_ref = decltype(std::declval<head<Inners...>>().front());
      static constexpr int inners_count = sizeof...(Inners);

    private:
      tuple data;
      int current = 0;

      using sp_ops_table_t =
          sp_ops<std::index_sequence_for<Inners...>, Inners...>;
      static constexpr sp_ops_table_t sp_ops_table = {};

    protected:
      const tuple& inners() const { return data; }
      tuple& inners() { return data; }

      concat_range_impl(Inners... inners) : data(inners...) { skip_empty(); }
      ~concat_range_impl() = default;

    public:
      using value_type = typename head<Inners...>::value_type;
      static_assert(all<std::is_same<value_type,
                                     typename Inners::value_type
                                    >::value...>::value,
                    "all concatenated ranges must have the same value type");

      bool empty() const {
        return current >= inners_count || sp_ops_table[current].empty(inners());
      }

      value_ref front() const {
        assert(!empty());
        return sp_ops_table[current].front(inners());
      }

      void drop_front() {
        assert(!empty());
        sp_ops_table[current].drop_front(inners());
        skip_empty();
      }

    private:
      void skip_empty() {
        while (current < inners_count && sp_ops_table[current].empty(inners())){
          ++current;
        }
      }
    };

    template <typename Derived, typename... Inners>
    constexpr typename concat_range_impl<Derived, single_pass_traversal_tag,
                                         Inners...>::sp_ops_table_t
    concat_range_impl<Derived, single_pass_traversal_tag,
                      Inners...>::sp_ops_table;

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
