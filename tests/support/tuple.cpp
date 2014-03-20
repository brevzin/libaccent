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

TEST(Variadics, ZipTuples) {
  auto t1 = std::make_tuple(42, 19); // values
  int i = 1, j = 9;
  auto t2 = std::tie(i, j); // references
  auto z1 = zip_tuples(t1, t2);
  auto z2 = zip_tuples(std::make_tuple(42, 19), std::tie(i, j));
  static_assert(std::is_same<std::tuple<
                                 std::tuple<int&, int&>,
                                 std::tuple<int&, int&>>,
                             decltype(z1)>::value,
      "zip of tuple lvalues should yield only lvalue refs");
  static_assert(std::is_same<std::tuple<
                                 std::tuple<int, int&>,
                                 std::tuple<int, int&>>,
                             decltype(z2)>::value,
      "zip of tuple rvalues should preserve element types");
  ASSERT_EQ(42, std::get<0>(std::get<0>(z1)));
  ASSERT_EQ(1, std::get<1>(std::get<0>(z1)));
  ASSERT_EQ(19, std::get<0>(std::get<1>(z1)));
  ASSERT_EQ(9, std::get<1>(std::get<1>(z1)));

  ASSERT_EQ(42, std::get<0>(std::get<0>(z2)));
  ASSERT_EQ(1, std::get<1>(std::get<0>(z2)));
  ASSERT_EQ(19, std::get<0>(std::get<1>(z2)));
  ASSERT_EQ(9, std::get<1>(std::get<1>(z2)));
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
