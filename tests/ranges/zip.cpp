#include <accent/ranges/zip.hpp>
#include <accent/algorithms/copy.hpp>
#include <accent/ranges/append.hpp>
#include "test.hpp"

using zip_shortest_sp_sp = decltype(zip_shortest(vec{}.sp(), vec{}.sp()));
using zip_shortest_sp_fwd = decltype(zip_shortest(vec{}.sp(), vec{}.fwd()));
using zip_shortest_fwd_sp = decltype(zip_shortest(vec{}.fwd(), vec{}.sp()));

static_assert(SinglePassRange<zip_shortest_sp_sp>(),
              "truncating zip of 2 single pass ranges "
              "is not single pass range");
static_assert(SinglePassRange<zip_shortest_sp_fwd>(),
              "truncating zip of single pass range and forward range "
              "is not single pass range");
static_assert(SinglePassRange<zip_shortest_fwd_sp>(),
              "truncating zip of forward range and single pass range "
              "is not single pass range");
static_assert(!ForwardRange<zip_shortest_sp_fwd>(),
              "truncating zip of single pass range and forward range "
              "claims to be forward range");
static_assert(!ForwardRange<zip_shortest_fwd_sp>(),
              "truncating zip of forward range and single pass range "
              "claims to be forward range");

using zip_shortest_fwd_fwd = decltype(zip_shortest(vec{}.fwd(), vec{}.fwd()));

static_assert(ForwardRange<zip_shortest_fwd_fwd>(),
              "truncating zip of 2 forward ranges "
              "is not forward range");

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

TEST(ZipShortest, swap) {
  vec v1 = { 1, 2 }, v2 = { -1, -2 };
  auto r1 = zip_shortest(v1.sp(), v2.sp());
  auto r2 = zip_shortest(v1.sp(), v2.sp());
  r2.drop_front();
  using std::swap;
  swap(r1.front(), r2.front());
  ASSERT_EQ((vec{2, 1}), v1);
  ASSERT_EQ((vec{-2, -1}), v2);

  auto t1 = std::make_tuple(3, -3);
  std::tuple<int, int> t0 = r1.front();
  swap(r1.front(), t1);
  ASSERT_EQ((vec{3, 1}), v1);
  ASSERT_EQ((vec{-3, -1}), v2);
  ASSERT_EQ(t0, t1);

  t0 = r2.front();
  swap(t1, r2.front());
  ASSERT_EQ((vec{3, 2}), v1);
  ASSERT_EQ((vec{-3, -2}), v2);
  ASSERT_EQ(t0, t1);
}

TEST(ZipShortest, empty_at_front_invalid) {
  vec v1 = {}, v2 = {};
  auto r = zip_shortest(v1.fwd(), v2.fwd());
  ASSERT_FALSE(r.at_front().valid());
}

TEST(ZipShortest, one_empty_at_front_invalid) {
  vec v1 = { 1 }, v2 = {};
  auto r = zip_shortest(v1.fwd(), v2.fwd());
  ASSERT_FALSE(r.at_front().valid());
}

TEST(ZipShortest, empty_one_at_front_invalid) {
  vec v1 = {}, v2 = { -1 };
  auto r = zip_shortest(v1.fwd(), v2.fwd());
  ASSERT_FALSE(r.at_front().valid());
}

TEST(ZipShortest, one_at_front_references_element) {
  vec v1 = { 1 }, v2 = { -1 };
  auto r = zip_shortest(v1.fwd(), v2.fwd());
  auto p = r.at_front();
  ASSERT_TRUE(p.valid());
  ASSERT_EQ(std::make_tuple(1, -1), *p);
}
