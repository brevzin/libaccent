#ifndef LIBACCENT_SUPPORT_TUPLE_HPP
#define LIBACCENT_SUPPORT_TUPLE_HPP

#include <tuple>
#include <type_traits>
#include <utility>

namespace accent { namespace support {

  namespace detail_variadics {

      template <typename Tuple>
      using tuple_index_sequence =
          std::make_index_sequence<std::tuple_size<Tuple>::value>;

      template <typename Fn, typename Tuple, std::size_t... Indices>
      decltype(auto) apply_tuple_impl(Fn&& fn, Tuple&& args,
                                      std::index_sequence<Indices...>) {
        return std::forward<Fn>(fn)(std::get<Indices>(args)...);
      }

  }

  template <typename Fn, typename Tuple>
  decltype(auto) apply_tuple(Fn&& fn, Tuple&& args) {
    return detail_variadics::apply_tuple_impl(
        std::forward<Fn>(fn),
        std::forward<Tuple>(args),
        detail_variadics::tuple_index_sequence<
            typename std::decay<Tuple>::type>());
  }

  template <typename Fn>
  class map_variadic_t {
    Fn fn;

  public:
    explicit map_variadic_t(Fn fn) : fn(std::move(fn)) {}
    template <typename... Args>
    auto operator ()(Args... args) const {
      return std::make_tuple(fn(args)...);
    }
  };

  template <typename Fn, typename Tuple>
  auto map_tuple(Fn fn, Tuple&& args) {
    return apply_tuple(map_variadic_t<Fn>(fn), std::forward<Tuple>(args));
  }

  template <typename Fn>
  class for_variadic_t {
    Fn fn;

  public:
    explicit for_variadic_t(Fn fn) : fn(std::move(fn)) {}
    template <typename... Args>
    void operator ()(Args... args) const {
      using ignore = int[];
      (void)ignore{(fn(args), 0)...};
    }
  };

  template <typename Fn, typename Tuple>
  void for_tuple(Fn fn, Tuple&& args) {
    apply_tuple(for_variadic_t<Fn>(fn), std::forward<Tuple>(args));
  }

  template <typename Fn>
  class reduce_variadic_t {
    Fn fn;

  public:
    explicit reduce_variadic_t(Fn fn) : fn(std::move(fn)) {}
    template <typename Arg, typename... Args>
    auto operator ()(Arg arg, Args... args) const {
      return fn(arg, (*this)(args...));
    }
    template <typename Arg>
    auto operator ()(Arg arg) const {
      return arg;
    }
  };

  template <typename Fn, typename Tuple>
  auto reduce_tuple(Fn fn, Tuple&& args) {
    return apply_tuple(reduce_variadic_t<Fn>(fn), std::forward<Tuple>(args));
  }

}}

#endif
