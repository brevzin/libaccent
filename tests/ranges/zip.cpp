#include <accent/ranges/zip.hpp>
#include <accent/algorithms/copy.hpp>
#include <accent/ranges/append.hpp>
#include "test.hpp"

using zip_shortest_sp_sp = decltype(zip_shortest(vec{}.sp(), vec{}.sp()));

static_assert(SinglePassRange<zip_shortest_sp_sp>(),
              "truncating zip of 2 single pass ranges "
              "is not single pass range");

using zsr_result = std::vector<std::tuple<int, int>>;
using zsr_spec = std::tuple<vec, vec, zsr_result>;

class ZipShortestRead : public ::testing::TestWithParam<zsr_spec> {};

TEST_P(ZipShortestRead, def) {
  auto p = GetParam();
  auto v1 = std::get<0>(p);
  auto v2 = std::get<1>(p);
  auto vr = std::get<2>(p);
  decltype(vr) va;
  copy(zip_shortest(v1.sp(), v2.sp()), append(va));
  ASSERT_EQ(vr, va);
}

INSTANTIATE_TEST_CASE_P(variousVectors, ZipShortestRead,
  ::testing::Values(zsr_spec{vec{}, vec{}, zsr_result{}},
    zsr_spec{vec{1}, vec{}, zsr_result{}},
    zsr_spec{vec{}, vec{-1}, zsr_result{}},
    zsr_spec{vec{1}, vec{-1}, zsr_result{{1, -1}}},
    zsr_spec{vec{1, 2}, vec{-1}, zsr_result{{1, -1}}},
    zsr_spec{vec{1}, vec{-1, -2}, zsr_result{{1, -1}}},
    zsr_spec{vec{1, 2}, vec{-1, -2}, zsr_result{{1, -1}, {2, -2}}}
  ));
