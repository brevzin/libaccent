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

using zsw_source = std::vector<std::tuple<int, int>>;
using zsw_spec = std::tuple<vec, vec, zsw_source, vec, vec>;

class ZipShortestWrite : public ::testing::TestWithParam<zsw_spec> {};

TEST_P(ZipShortestWrite, def) {
  auto p = GetParam();
  auto v1 = std::get<0>(p);
  auto v2 = std::get<1>(p);
  auto vp = std::get<2>(p);
  copy(adapt(vp).all(), zip_shortest(v1.sp(), v2.sp()));
  ASSERT_EQ(std::get<3>(p), v1);
  ASSERT_EQ(std::get<4>(p), v2);
}

INSTANTIATE_TEST_CASE_P(variousVectors, ZipShortestWrite,
  ::testing::Values(zsw_spec{vec{}, vec{}, zsw_source{}, vec{}, vec{}},
    zsw_spec{vec{0}, vec{}, zsw_source{{1, -1}}, vec{0}, vec{}},
    zsw_spec{vec{}, vec{0}, zsw_source{{1, -1}}, vec{}, vec{0}},
    zsw_spec{vec{0}, vec{0}, zsw_source{{1, -1}}, vec{1}, vec{-1}},
    zsw_spec{vec{0, 0}, vec{0}, zsw_source{{1, -1}}, vec{1, 0}, vec{-1}},
    zsw_spec{vec{0}, vec{0, 0}, zsw_source{{1, -1}}, vec{1}, vec{-1, 0}},
    zsw_spec{vec{0, 0}, vec{0, 0}, zsw_source{{1, -1}}, vec{1, 0}, vec{-1, 0}},
    zsw_spec{vec{0, 0}, vec{0, 0}, zsw_source{{1, -1}, {2, -2}},
             vec{1, 2}, vec{-1, -2}}
  ));
