#ifndef LIBACCENT_TESTS_TEST_HPP
#define LIBACCENT_TESTS_TEST_HPP

#include <accent/ranges/as.hpp>
#include <accent/ranges/vector_range.hpp>
#include <accent/support/concepts.hpp>
#include <gtest/gtest.h>
#include <initializer_list>
#include <tuple>
#include <vector>

namespace accent {
  namespace algorithms {}
  namespace ranges {}
  namespace support {}
}

using namespace accent;
using namespace accent::algorithms;
using namespace accent::ranges;
using namespace accent::support;

class vec {
  std::vector<int> v;

public:
  vec() {}
  vec(std::initializer_list<int> l) : v(l) {}

  const std::vector<int>& get() const { return v; }

  auto sp() const -> decltype(as_single_pass(adapt(v).all())) {
    return as_single_pass(adapt(v).all());
  }
  auto sp() -> decltype(as_single_pass(adapt(v).all())) {
    return as_single_pass(adapt(v).all());
  }
  auto fwd() const -> decltype(as_forward(adapt(v).all())) {
    return as_forward(adapt(v).all());
  }
  auto fwd() -> decltype(as_forward(adapt(v).all())) {
    return as_forward(adapt(v).all());
  }
  auto bidi() const -> decltype(as_bidirectional(adapt(v).all())) {
    return as_bidirectional(adapt(v).all());
  }
  auto bidi() -> decltype(as_bidirectional(adapt(v).all())) {
    return as_bidirectional(adapt(v).all());
  }
};

inline bool operator ==(const vec& lhs, const vec& rhs) {
  return lhs.get() == rhs.get();
}

inline void PrintTo(const vec& v, std::ostream* os) {
  *os << ::testing::PrintToString(v.get());
}

namespace test_detail {
  template <typename T>
  void print_one(std::ostream* os, const T& t) {
    *os << ", " << ::testing::PrintToString(t);
  }

  template <typename... T>
  void ignore(T...) {}

  template <typename Tuple, std::size_t... Indices>
  void do_print(std::ostream* os, const Tuple& t,
                std::index_sequence<0, Indices...>) {
    ignore((print_one(os, std::get<Indices>(t)), 0)...);
  }
}

namespace std { // evil!
  template <typename... Ts>
  inline void PrintTo(const tuple<Ts...>& t, ostream* os) {
    *os << "{ " << ::testing::PrintToString(get<0>(t));
    ::test_detail::do_print(os, t, index_sequence_for<Ts...>{});
    *os << " }";
  }
}

struct positive {
  bool operator ()(int i) const { return i > 0; }
};

struct tracking_positive {
  int& tracker;
  tracking_positive(int& tracker) : tracker(tracker) {}
  bool operator ()(int i) const { ++tracker; return i > 0; }
};

#endif
