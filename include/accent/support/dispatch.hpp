#ifndef LIBACCENT_SUPPORT_DISPATCH_HPP
#define LIBACCENT_SUPPORT_DISPATCH_HPP

// A little generic facility to dispatch functors to an element of a
// heterogeneous container by a runtime index.

#include <cassert>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace accent { namespace support {

  namespace detail {

    // To start ADL, here's a bogus overload.
    struct bogus {};
    template <std::size_t> void get(bogus);

    template <std::size_t I, typename T>
    auto adl_get(T& t) -> decltype(get<I>(t));

    template <typename Indices, typename Container, typename Operation>
    struct dispatch_table;

    template <std::size_t... Indices, typename Container, typename Operation>
    struct dispatch_table<std::index_sequence<Indices...>, Container, Operation> {
      using result = decltype(std::declval<Operation>()(
          get<0>(std::declval<Container&>())));

      template <std::size_t I>
      static result call(Operation op, Container& v) {
        return op(get<I>(v));
      }

      using pointer = result (*)(Operation op, Container&);
      static constexpr pointer table[] = {
        &call<Indices>...
      };
      static constexpr int size = sizeof...(Indices);
    };

    template <std::size_t... Indices, typename Container, typename Operation>
    constexpr typename dispatch_table<std::index_sequence<Indices...>,
                                      Container, Operation>::pointer
    dispatch_table<std::index_sequence<Indices...>,
                   Container, Operation>::table[];

    template <typename Container>
    struct index_sequence_for_container;

    template <template <typename...> class Container, typename... Elements>
    struct index_sequence_for_container<Container<Elements...>> {
      using type = std::index_sequence_for<Elements...>;
    };

  }

  template <typename Container, typename Operation>
  auto dispatch(Container& c, Operation op, int index)
      -> decltype(op(detail::adl_get<0>(c))) {
    using table = detail::dispatch_table<
        typename detail::index_sequence_for_container<
            typename std::remove_const<Container>::type>::type,
        Container, Operation>;
    assert(index >= 0 && index < table::size);
    return table::table[index](op, c);
  }

}}

#endif
