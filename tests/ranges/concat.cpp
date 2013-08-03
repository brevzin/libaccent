#include <accent/ranges/concat.hpp>
#include "test.hpp"

using concat_sp_sp = decltype(concat(vec{}.sp(), vec{}.sp()));
using concat_sp_sp_sp = decltype(concat(vec{}.sp(), vec{}.sp(), vec{}.sp()));

static_assert(SinglePassRange<concat_sp_sp>(),
              "concatenation of 2 single pass ranges is not single pass range");
static_assert(SinglePassRange<concat_sp_sp_sp>(),
              "concatenation of 3 single pass ranges is not single pass range");

TEST(Concat, EmptyPlusEmpty_Empty) {
  vec v1 = {}, v2 = {};
  auto r = concat(v1.sp(), v2.sp());
  ASSERT_TRUE(r.empty());
}

TEST(Concat, OnePlusEmpty_One) {
  vec v1 = { 1 }, v2 = {};
  auto r = concat(v1.sp(), v2.sp());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(1, r.front());
  r.drop_front();
  ASSERT_TRUE(r.empty());
}

TEST(Concat, EmptyPlusOne_One) {
  vec v1 = {}, v2 = { 1 };
  auto r = concat(v1.sp(), v2.sp());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(1, r.front());
  r.drop_front();
  ASSERT_TRUE(r.empty());
}

TEST(Concat, OnePlusOne_Two) {
  vec v1 = { 1 }, v2 = { 2 };
  auto r = concat(v1.sp(), v2.sp());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(1, r.front());
  r.drop_front();
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(2, r.front());
  r.drop_front();
  ASSERT_TRUE(r.empty());
}

TEST(Concat, TwoPlusOne_Three) {
  vec v1 = { 1, 2 }, v2 = { 3 };
  auto r = concat(v1.sp(), v2.sp());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(1, r.front());
  r.drop_front();
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(2, r.front());
  r.drop_front();
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(3, r.front());
  r.drop_front();
  ASSERT_TRUE(r.empty());
}
