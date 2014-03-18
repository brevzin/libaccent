#include <accent/support/tuple.hpp>
#include <functional>
#include <sstream>
#include "test.hpp"

TEST(Variadics, ApplyTuple) {
  auto fn = [](int i, double d) { return i - d; };
  auto tuple = std::make_tuple(5, 2.5);
  ASSERT_EQ(2.5, apply_tuple(fn, tuple));
}

TEST(Variadics, MapTuple) {
  struct Fn { template <typename T> T operator ()(T t) const { return t / 2; } }
  fn;
  auto tuple = std::make_tuple(5, 5.0);
  ASSERT_EQ(std::make_tuple(2, 2.5), map_tuple(fn, tuple));
  static_assert(std::is_same<std::tuple<int, double>,
                             decltype(map_tuple(fn, tuple))>::value,
                "Result doesn't capture values.");
  struct Fn2 { template <typename T> T& operator ()(T& t) const { return t; } };
  static_assert(std::is_same<std::tuple<int&, double&>,
                             decltype(map_tuple(Fn2(), tuple))>::value,
                "Result doesn't capture lvalue references.");
}

TEST(Variadics, ForTuple) {
  std::ostringstream s;
  struct Fn {
    std::ostringstream& s;
    template <typename T> void operator ()(T t) const {
      s << t;
    }
  };
  Fn fn{s};
  auto tuple = std::make_tuple(5, 2.5, ' ');
  for_tuple(fn, tuple);
  ASSERT_EQ("52.5 ", s.str());
}

TEST(Variadics, ForTupleN) {
  std::ostringstream s;
  struct Fn {
    std::ostringstream& s;
    template <typename T, typename U>
    void operator ()(T t, U u) const {
      s << t << ":" << u << " ";
    }
  };
  Fn fn{s};
  auto tuple1 = std::make_tuple(5, 2.5, 'x');
  auto tuple2 = std::make_tuple(1.5, 't', 9);
  for_tuple(fn, tuple1, tuple2);
  ASSERT_EQ("5:1.5 2.5:t x:9 ", s.str());
}

TEST(Variadics, ReduceTuple) {
  ASSERT_TRUE(reduce_tuple(std::logical_and<>(),
                           std::make_tuple(true, true, true)));
  ASSERT_FALSE(reduce_tuple(std::logical_and<>(),
                            std::make_tuple(true, true, false)));
  ASSERT_TRUE(reduce_tuple(std::logical_or<>(),
                           std::make_tuple(true, false, false)));
  ASSERT_FALSE(reduce_tuple(std::logical_or<>(),
                            std::make_tuple(false, false, false)));
}
