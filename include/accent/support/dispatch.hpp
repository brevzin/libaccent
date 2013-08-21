#ifndef LIBACCENT_SUPPORT_DISPATCH_HPP
#define LIBACCENT_SUPPORT_DISPATCH_HPP

// A little generic facility to dispatch functors to an element of a
// heterogeneous container by a runtime index.

#include <cassert>
#include <type_traits>
#include <utility>

namespace accent { namespace support {

  namespace detail {

    template <typename Indices, typename Container, typename Operation>
    struct dispatch_table;

    template <std::size_t... Indices, typename Container, typename Operation>
    struct dispatch_table<std::index_sequence<Indices...>, Container, Operation> {
      using result = decltype(std::declval<Operation>()(
          std::declval<Container&>().template get<0>()));

      template <std::size_t I>
      static result call(Operation op, Container& v) {
        return op(v.template get<I>());
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
      -> decltype(op(c.template get<0>())) {
    using table = detail::dispatch_table<
        typename detail::index_sequence_for_container<
            typename std::remove_const<Container>::type>::type,
        Container, Operation>;
    assert(index >= 0 && index < table::size);
    return table::table[index](op, c);
  }

}}

#endif
