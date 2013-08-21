#ifndef LIBACCENT_SUPPORT_VARIANT_HPP
#define LIBACCENT_SUPPORT_VARIANT_HPP

#include "accent/support/dispatch.hpp"
#include "accent/support/utility.hpp"
#include <cassert>
#include <new>
#include <tuple>
#include <type_traits>
#include <utility>

namespace accent { namespace support {

  namespace detail {

    template <bool, template <typename> class Predicate,
              typename Element, typename... Elements>
    struct first_impl;

    template <template <typename> class Predicate,
              typename Element, typename NextElement, typename... Elements>
    struct first_impl<false, Predicate, Element, NextElement, Elements...> {
      using type = typename first_impl<Predicate<NextElement>{}, Predicate,
                                       NextElement, Elements...>::type;
    };

    template <template <typename> class Predicate,
              typename Element, typename... Elements>
    struct first_impl<true, Predicate, Element, Elements...> {
      using type = Element;
    };

    template <template <typename> class Predicate,
              typename Element, typename... Elements>
    using first = typename first_impl<Predicate<Element>{}, Predicate,
                                      Element, Elements...>::type;

    template <typename T>
    struct is_default_type {
      constexpr operator bool() const {
        return std::is_nothrow_default_constructible<T>::value;
      }
    };

    template <std::size_t... Values> struct max;

    template <> struct max<> { constexpr operator int() const { return 0; } };
    template <std::size_t V, std::size_t... Rem>
    struct max<V, Rem...> {
      using prev = max<Rem...>;
      constexpr operator int() const {
        return V > prev() ? V : prev();
      }
    };

    template <int I, typename Needle, typename... Haystack> struct index_impl;
    template <int I, typename Needle>
    struct index_impl<I, Needle> {};
    template <int I, typename Needle, typename... Dull>
    struct index_impl<I, Needle, Needle, Dull...> {
      constexpr operator int() const { return I; }
    };
    template <int I, typename Needle, typename NotIt, typename... Rest>
    struct index_impl<I, Needle, NotIt, Rest...>
        : index_impl<I + 1, Needle, Rest...>
    {};

    template <typename Needle, typename... Haystack>
    struct index : index_impl<0, Needle, Haystack...> {};

    class copy_op {
      void* destination;

    public:
      explicit copy_op(void* destination) : destination(destination) {}

      template <typename T>
      void operator ()(const T& t) const {
        new (destination) T(t);
      }
    };

    template <typename Variant>
    class copy_assign_op {
      const Variant& source;

    public:
      explicit copy_assign_op(const Variant& source) : source(source) {}

      template <typename T>
      void operator ()(T& t) const {
        t = source.template get<T>();
      }
    };

    template <typename Variant>
    copy_assign_op<Variant> make_copy_assign_op(const Variant& source) {
      return copy_assign_op<Variant>{source};
    }

    class move_op {
      void* destination;

    public:
      explicit move_op(void* destination) : destination(destination) {}

      template <typename T>
      void operator ()(T& t) const {
        new (destination) T(std::move(t));
      }
    };

    struct destruct_op {
      template <typename T>
      void operator ()(T& t) const {
        t.~T();
      }
    };

    template <typename Variant>
    class move_assign_op {
      Variant& source;

    public:
      explicit move_assign_op(Variant&& source) : source(source) {}

      template <typename T>
      void operator ()(T& t) const {
        t = std::move(source.template get<T>());
      }
    };

    template <typename Variant>
    move_assign_op<Variant> make_move_assign_op(Variant&& source) {
      return move_assign_op<Variant>{std::move(source)};
    }

  }

  template <typename... Types>
  class variant {
    // If this line fails to compile, you did not supply at least one nothrow-
    // constructible type.
    using default_type = detail::first<detail::is_default_type, Types...>;
    using tuple = std::tuple<Types...>;
    using storage_t = typename std::aligned_union<
                          (detail::max<sizeof(Types)...>()), Types...>::type;
    storage_t storage;
    int tag;

  public:
    variant() noexcept : variant(default_type{}) {}

    template <typename T,
              typename = typename std::enable_if<
                             detail::index<T, Types...>() != -1>::type>
    variant(T t) : tag(detail::index<T, Types...>()) {
      new (&storage) T(std::move(t));
    }

    variant(const variant& o)
        : tag(o.tag) {
      o.apply(detail::copy_op(&storage));
    }

    // Exceptions: weak
    template <typename T,
              typename = typename std::enable_if<
                             detail::index<T, Types...>() != -1>::type>
    variant& operator =(T t) {
      destroy();
      try {
        new (&storage) T(std::move(t));
        tag = detail::index<T, Types...>();
      } catch (...) {
        revert();
        throw;
      }
      return *this;
    }

    // Exceptions: weak
    variant& operator =(const variant& o) {
      if (which() == o.which()) {
        apply(detail::make_copy_assign_op(o));
      } else {
        destroy();
        try {
          o.apply(detail::copy_op(&storage));
          tag = o.tag;
        } catch (...) {
          revert();
          throw;
        }
      }
      return *this;
    }

    variant(variant&& o)
      noexcept(all<std::is_nothrow_move_constructible<Types>::value...>::value)
        : tag(o.tag) {
      o.apply(detail::move_op(&storage));
    }

    // Exceptions: weak
    variant& operator =(variant&& o)
      noexcept(all<(std::is_nothrow_move_constructible<Types>::value &&
                    std::is_nothrow_move_assignable<Types>::value)...>::value) {
      if (which() == o.which()) {
        apply(detail::make_move_assign_op(std::move(o)));
      } else {
        destroy();
        try {
          o.apply(detail::move_op(&storage));
          tag = o.tag;
        } catch (...) {
          revert();
          throw;
        }
      }
      return *this;
    }

    ~variant() { destroy(); }

    int which() const {
      return tag;
    }

    template <std::size_t I>
    auto get() -> typename std::tuple_element<I, tuple>::type& {
      assert(which() == I && "wrong active member");
      return reinterpret_cast<
          typename std::tuple_element<I, tuple>::type&>(storage);
    }

    template <std::size_t I>
    auto get() const -> const typename std::tuple_element<I, tuple>::type& {
      assert(which() == I && "wrong active member");
      return reinterpret_cast<
          const typename std::tuple_element<I, tuple>::type&>(storage);
    }

    template <typename T>
    T& get() {
      assert((which() == detail::index<T, Types...>()) &&
             "wrong active member");
      return reinterpret_cast<T&>(storage);
    }

    template <typename T>
    const T& get() const {
      assert((which() == detail::index<T, Types...>()) &&
             "wrong active member");
      return reinterpret_cast<const T&>(storage);
    }

    template <typename Operation>
    auto apply(Operation op) {
      return dispatch(*this, op, which());
    }
    template <typename Operation>
    auto apply(Operation op) const {
      return dispatch(*this, op, which());
    }

  private:
    void destroy() {
      apply(detail::destruct_op());
    }

    void revert() {
      new (&storage) default_type{};
      tag = detail::index<default_type, Types...>();
    }
  };

}}

#endif
