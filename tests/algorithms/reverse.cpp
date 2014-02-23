#include <accent/algorithms/reverse.hpp>
#include <utility>
#include "test.hpp"

using vecp = std::pair<vec, vec>;

class Reverse : public ::testing::TestWithParam<vecp> {};

TEST_P(Reverse, bidi) {
  auto p = GetParam();
  auto r = p.first.bidi();
  reverse(r);
  ASSERT_EQ(p.second, p.first);
}

INSTANTIATE_TEST_CASE_P(variousVectors, Reverse,
  ::testing::Values(vecp{{}, {}}, vecp{{1}, {1}}, vecp{{1, 2}, {2, 1}},
    vecp{{1, 2, 3}, {3, 2, 1}}, vecp{{1, 2, 3, 4}, {4, 3, 2, 1}},
    vecp{{1, 2, 3, 4, 5}, {5, 4, 3, 2, 1}}));

using spec = std::tuple<vec, int, vec>;

class ReverseWithPosition : public ::testing::TestWithParam<spec> {};

TEST_F(ReverseWithPosition, empty) {
  vec v{};
  auto pos = reverse_with_position(v.bidi(), v.bidi().at_front());
  ASSERT_FALSE(pos.valid());
}

TEST_P(ReverseWithPosition, bidi) {
  auto p = GetParam();
  auto r = std::get<0>(p).bidi();
  auto s = r;
  for (int i = 0; i < std::get<1>(p); ++i) {
    s.drop_front();
  }
  auto previous = s.front();
  auto pos = reverse_with_position(r, s.at_front());
  ASSERT_EQ(std::get<2>(p), std::get<0>(p));
  ASSERT_TRUE(pos.valid());
  ASSERT_EQ(previous, *pos);
}

INSTANTIATE_TEST_CASE_P(variousVectors, ReverseWithPosition,
  ::testing::Values(spec{{1}, 0, {1}},
    spec{{1, 2}, 0, {2, 1}}, spec{{1, 2}, 1, {2, 1}},
    spec{{1, 2, 3}, 0, {3, 2, 1}}, spec{{1, 2, 3}, 1, {3, 2, 1}},
    spec{{1, 2, 3}, 2, {3, 2, 1}}));
