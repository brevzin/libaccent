#include <iostream>
#include <accent/algorithms/rotate.hpp>
#include <tuple>
#include "test.hpp"

using spec = std::tuple<vec, int, vec>;

class Rotate : public ::testing::TestWithParam<spec> {};

TEST_F(Rotate, fwd_empty) {
  vec v;
  auto np = rotate(v.fwd(), v.fwd().at_front());
  ASSERT_FALSE(np.valid());
}

TEST_P(Rotate, fwd) {
  auto p = GetParam();
  auto source = std::get<0>(p);
  auto rotated = source;
  auto r = rotated.fwd();
  auto s = r;
  for (int i = 0; i < std::get<1>(p); ++i) {
    s.drop_front();
  }
  auto np = rotate(r, s.at_front());
  auto& expected = std::get<2>(p);
  ASSERT_EQ(expected, rotated);
  ASSERT_TRUE(np.valid());
  ASSERT_EQ(source.get()[0], *np);
}

TEST_P(Rotate, bidi) {
  auto p = GetParam();
  auto source = std::get<0>(p);
  auto rotated = source;
  auto r = rotated.bidi();
  auto s = r;
  for (int i = 0; i < std::get<1>(p); ++i) {
    s.drop_front();
  }
  auto np = rotate(r, s.at_front());
  auto& expected = std::get<2>(p);
  ASSERT_EQ(expected, rotated);
  ASSERT_TRUE(np.valid());
  ASSERT_EQ(source.get()[0], *np);
}

INSTANTIATE_TEST_CASE_P(variousVectors, Rotate,
  ::testing::Values(spec({1}, 0, {1}),
    spec({1, 2}, 0, {1, 2}), spec({1, 2}, 1, {2, 1}),
    spec({1, 2, 3, 4, 5, 6, 7, 8}, 0, {1, 2, 3, 4, 5, 6, 7, 8}),
    spec({1, 2, 3, 4, 5, 6, 7, 8}, 1, {2, 3, 4, 5, 6, 7, 8, 1}),
    spec({1, 2, 3, 4, 5, 6, 7, 8}, 2, {3, 4, 5, 6, 7, 8, 1, 2}),
    spec({1, 2, 3, 4, 5, 6, 7, 8}, 3, {4, 5, 6, 7, 8, 1, 2, 3}),
    spec({1, 2, 3, 4, 5, 6, 7, 8}, 4, {5, 6, 7, 8, 1, 2, 3, 4}),
    spec({1, 2, 3, 4, 5, 6, 7, 8}, 5, {6, 7, 8, 1, 2, 3, 4, 5}),
    spec({1, 2, 3, 4, 5, 6, 7, 8}, 6, {7, 8, 1, 2, 3, 4, 5, 6}),
    spec({1, 2, 3, 4, 5, 6, 7, 8}, 7, {8, 1, 2, 3, 4, 5, 6, 7}),
    spec({1, 2, 3, 4, 5, 6, 7, 8, 9}, 0, {1, 2, 3, 4, 5, 6, 7, 8, 9}),
    spec({1, 2, 3, 4, 5, 6, 7, 8, 9}, 1, {2, 3, 4, 5, 6, 7, 8, 9, 1}),
    spec({1, 2, 3, 4, 5, 6, 7, 8, 9}, 2, {3, 4, 5, 6, 7, 8, 9, 1, 2}),
    spec({1, 2, 3, 4, 5, 6, 7, 8, 9}, 3, {4, 5, 6, 7, 8, 9, 1, 2, 3}),
    spec({1, 2, 3, 4, 5, 6, 7, 8, 9}, 4, {5, 6, 7, 8, 9, 1, 2, 3, 4}),
    spec({1, 2, 3, 4, 5, 6, 7, 8, 9}, 5, {6, 7, 8, 9, 1, 2, 3, 4, 5}),
    spec({1, 2, 3, 4, 5, 6, 7, 8, 9}, 6, {7, 8, 9, 1, 2, 3, 4, 5, 6}),
    spec({1, 2, 3, 4, 5, 6, 7, 8, 9}, 7, {8, 9, 1, 2, 3, 4, 5, 6, 7}),
    spec({1, 2, 3, 4, 5, 6, 7, 8, 9}, 8, {9, 1, 2, 3, 4, 5, 6, 7, 8})));
