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
